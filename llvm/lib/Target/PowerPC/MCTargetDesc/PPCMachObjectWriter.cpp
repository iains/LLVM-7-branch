//===-- PPCMachObjectWriter.cpp - PPC Mach-O Writer -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/PPCFixupKinds.h"
#include "MCTargetDesc/PPCMCTargetDesc.h"
#include "llvm/ADT/Twine.h"
#include "llvm/BinaryFormat/MachO.h"
#include "llvm/MC/MCAsmLayout.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCMachObjectWriter.h"
#include "llvm/MC/MCSectionMachO.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/Format.h"

#include "llvm/Support/Debug.h"

#define DEBUG_TYPE "macho-relocs"

using namespace llvm;

namespace {
class PPCMachObjectWriter : public MCMachObjectTargetWriter {
  void recordScatteredRelocation(MachObjectWriter *Writer,
                                 const MCAssembler &Asm,
                                 const MCAsmLayout &Layout,
                                 const MCFragment *Fragment,
                                 const MCFixup &Fixup, MCValue Target,
                                 uint64_t &FixedValue, const MCFixupKind FK);

  void recordRegularRelocation(MachObjectWriter *Writer,
                               const MCAssembler &Asm,
                               const MCAsmLayout &Layout,
                               const MCFragment *Fragment,
                               const MCFixup &Fixup, MCValue Target,
                               uint64_t &FixedValue, const MCFixupKind FK);

public:
  PPCMachObjectWriter(bool Is64Bit, uint32_t CPUType, uint32_t CPUSubtype)
      : MCMachObjectTargetWriter(Is64Bit, CPUType, CPUSubtype) {}

  void recordRelocation(MachObjectWriter *Writer, MCAssembler &Asm,
                        const MCAsmLayout &Layout, const MCFragment *Fragment,
                        const MCFixup &Fixup, MCValue Target,
                        uint64_t &FixedValue) override {

    /// Mach-O can handle relocations of forms up to "A-B+offset" (with some
    /// limitations).  To do this (for ppc and ppc64) we use two kinds of
    /// relocation entry:
    /// 1. A "regular" type which is used for cases where there is no
    ///       subtracted symbol and:
    /// 1.a A is an undefined external symbol (optionally with an offset)
    /// 1.b A is a defined symbol without any offset.
    /// 1.c A is a defined symbol where the offset exceeds the value that can
    ///       be encoded in a scattered relocation - this is acknowledged as a
    ///       risky scenario, but isn't reported as a warning/error currently.

    /// 2. A "scattered" type which is used for all other cases, including
    /// those where there are subtractions of two symbols, although both of
    /// the symbols must be define in the TU for these to work.

    const MCFixupKind FK = Fixup.getKind();
    bool ADefined = false;
    const MCSymbol *A = nullptr;
    int32_t AOffs = 0;
    if (Target.getSymA())
      A = &Target.getSymA()->getSymbol();
    if (A != nullptr && A->getFragment() != nullptr) {
      ADefined = true;
      AOffs = Target.getConstant() + Layout.getFragmentOffset(Fragment);
    }

    if (!Target.getSymB() && 
         (!ADefined /*1.a*/ || Target.getConstant() == 0 /*1.b*/||
           AOffs > 0x00ffffff /*1.c*/))
      recordRegularRelocation(Writer, Asm, Layout, Fragment, Fixup, Target,
                              FixedValue, FK);
    else if (!ADefined)
      report_fatal_error("target symbol '"
                         + ((A != nullptr) ? A->getName() : "") +
                       "' can not be undefined in a subtraction expression");
    else /*2*/
      recordScatteredRelocation(Writer, Asm, Layout, Fragment, Fixup, Target,
                                FixedValue, FK);
  }
};
} // anonymous namespace.

// Supporting functions.

/// Translates generic PPC fixup kind to Mach-O/PPC relocation type enum.
static unsigned getRelocType(const MCValue &Target,
                             const MCFixupKind FixupKind, // from
                                                          // Fixup.getKind()
                             const bool IsPCRel) {
  const MCSymbolRefExpr::VariantKind Modifier =
      Target.isAbsolute() ? MCSymbolRefExpr::VK_None
                          : Target.getSymA()->getKind();
  // Determine the type of the relocation.
  // Default to vanilla.
  unsigned Type = MachO::PPC_RELOC_VANILLA;
  if (IsPCRel) {
    switch ((unsigned)FixupKind) {
    case PPC::fixup_ppc_br24:
      Type = MachO::PPC_RELOC_BR24; // R_PPC_REL24
      break;
    case PPC::fixup_ppc_brcond14:
      Type = MachO::PPC_RELOC_BR14;
      break;
    case FK_PCRel_8:
    case FK_PCRel_4:
    case FK_PCRel_2:
    case FK_PCRel_1:
      break;
     default:
      report_fatal_error("Unimplemented fixup kind (PC-relative)");
     break;
    }
  } else {
    switch ((unsigned)FixupKind) {
    case FK_Data_8:
    case FK_Data_4:
    case FK_Data_2:
    case FK_Data_1:
      break;
    case PPC::fixup_ppc_half16:
    case PPC::fixup_ppc_half16ds:
      switch (Modifier) {
      default:
        llvm_unreachable("Unsupported modifier for half16 fixup");
      case MCSymbolRefExpr::VK_PPC_HA:
        Type = MachO::PPC_RELOC_HA16;
        break;
      case MCSymbolRefExpr::VK_PPC_HI:
        Type = MachO::PPC_RELOC_HI16;
        break;
      case MCSymbolRefExpr::VK_PPC_LO:
        Type = (unsigned)FixupKind == PPC::fixup_ppc_half16ds
               ? MachO::PPC_RELOC_LO14
               : MachO::PPC_RELOC_LO16;
        break;
      }
      break;
    default:
      report_fatal_error("Unimplemented fixup kind (absolute)!");
      break;
    }
  }
  return Type;
}

/// computes the log2 of the size of the relocation,
/// used for relocation_info::r_length.
static unsigned getFixupKindLog2Size(unsigned Kind) {
  switch (Kind) {
  default:
    report_fatal_error("log2size(FixupKind): Unhandled fixup kind! (%d)", Kind);
  case FK_PCRel_1:
  case FK_Data_1:
    return 0;
  case FK_PCRel_2:
  case FK_Data_2:
    return 1;
  case FK_PCRel_4:
  case FK_Data_4:
  case PPC::fixup_ppc_brcond14:
  case PPC::fixup_ppc_br24:
  case PPC::fixup_ppc_half16:
  case PPC::fixup_ppc_half16ds:
    return 2;
  case FK_PCRel_8:
  case FK_Data_8:
    return 3;
  }
  return 0;
}

// See llvm/Support/Macho.h, 
// Mac OS X ABI Mach-O File Format Reference : Relocation Data Structures
// Mach-O Programming Topics : Position independent code.

// Here we shift the fields into position, avoiding the need to have
// endian-specific code to manipulate them.
static void makeRelocationInfo(MachO::any_relocation_info &MRE,
                               const uint32_t FixupOffset, const uint32_t Index,
                               const unsigned IsPCRel, const unsigned Log2Size,
                               const unsigned IsExtern, const unsigned Type) {
  MRE.r_word0 = FixupOffset;
  MRE.r_word1 = ((Index << 8) | // r_symbolnum = (extern)?symbol#:section#
                 (IsPCRel << 7) | // r_pcrel
                 (Log2Size << 5) | // r_size log2(size) #3 is special.
                 (IsExtern << 4) | // r_extern - symbol is in the symtab
                 (Type << 0)); // r_type = relocation type.
}

static void
makeScatteredRelocationInfo(MachO::any_relocation_info &MRE,
                            const uint32_t Addr, const unsigned Type,
                            const unsigned Log2Size, const unsigned IsPCRel,
                            const uint32_t Value2) {
  MRE.r_word0 = ((Addr << 0) |
                 (Type << 24) |
                 (Log2Size << 28) |
                 (IsPCRel << 30) |
                 MachO::R_SCATTERED);
  MRE.r_word1 = Value2;
}

/// Compute fixup offset (address).
static uint32_t getFixupOffset(const MCAsmLayout &Layout,
                               const MCFragment *Fragment,
                               const MCFixup &Fixup) {
  uint32_t FixupOffset = Layout.getFragmentOffset(Fragment) + Fixup.getOffset();
  // On Mach-O, ppc_fixup_half16 relocations must refer to the
  // start of the instruction, not the second halfword, as ELF does
  if (unsigned(Fixup.getKind()) == PPC::fixup_ppc_half16 ||
      unsigned(Fixup.getKind()) == PPC::fixup_ppc_half16ds)
    FixupOffset &= ~uint32_t(3);
  return FixupOffset;
}

/// Record a scattered relocation or fail.

void PPCMachObjectWriter::recordScatteredRelocation(
    MachObjectWriter *Writer, const MCAssembler &Asm, const MCAsmLayout &Layout,
    const MCFragment *Fragment, const MCFixup &Fixup, MCValue Target,
    uint64_t &FixedValue, const MCFixupKind FK ) {

  const uint32_t FixupOffset = getFixupOffset(Layout, Fragment, Fixup);
  const unsigned Log2Size = getFixupKindLog2Size(FK);
  const bool IsPCRel = Writer->isFixupKindPCRel(Asm, FK);
  unsigned Type = getRelocType(Target, FK, IsPCRel);
  bool IsBr = (unsigned) FK == PPC::fixup_ppc_br24 ||
              (unsigned) FK == PPC::fixup_ppc_brcond14;

  // We've already checked that A is defined.
  const MCSymbol *A = &Target.getSymA()->getSymbol();
  uint32_t Value = Writer->getSymbolAddress(*A, Layout);
  uint64_t SecAddr = Writer->getSectionAddress(A->getFragment()->getParent());
  FixedValue += SecAddr; // Address of A.
  uint32_t Value2 = 0;
  MachO::any_relocation_info MRE;

  if (const MCSymbolRefExpr *B = Target.getSymB()) {
    const MCSymbol *SB = &B->getSymbol();

    if (!SB->getFragment())
      report_fatal_error("symbol '" + SB->getName() +
                         "' can not be undefined in a subtraction expression");

    if (IsBr)
      report_fatal_error("symbol '" + SB->getName() +
                         "' can not be subtracted in a branch instruction");

    // Select the appropriate difference relocation type.
    //
    // Note that there is no longer any semantic difference between these two
    // relocation types from the linkers point of view, this is done solely
    // for pedantic compatibility with 'cctools as'.

    switch (Type) {
    case MachO::PPC_RELOC_VANILLA:
      Type = A->isExternal() ? (unsigned)MachO::PPC_RELOC_SECTDIFF
                             : (unsigned)MachO::PPC_RELOC_LOCAL_SECTDIFF;
      break;
    case MachO::PPC_RELOC_HI16:
      dbgs() << "The linker doesn't expect HI16_SECTDIFF scattered relocs?\n";
      Type = MachO::PPC_RELOC_HI16_SECTDIFF;
      break;
    case MachO::PPC_RELOC_HA16:
      Type = MachO::PPC_RELOC_HA16_SECTDIFF;
      break;
    case MachO::PPC_RELOC_LO16:
      Type = MachO::PPC_RELOC_LO16_SECTDIFF;
      break;
    case MachO::PPC_RELOC_LO14:
      Type = MachO::PPC_RELOC_LO14_SECTDIFF;
      break;
    default:
      dbgs() << "Unhandled relocation type in scattered relocs.\n";
      break;
    }

    // Address of B in its section.
    Value2 = Writer->getSymbolAddress(*SB, Layout);
    // Subtract out the section start for B.
    FixedValue -= Writer->getSectionAddress(SB->getFragment()->getParent());

    // Value = Addr (A)
    // Value2 = Addr (B)
    // FixedValue = A + sect(A) - sect(B).

    // If the offset is too large to fit in a scattered relocation,
    // we're hosed. It's an unfortunate limitation of the MachO format.
    if (FixupOffset > 0x00ffffff) {
      char Buffer[32];
      format("0x%x", FixupOffset).print(Buffer, sizeof(Buffer));
      Asm.getContext().reportError(Fixup.getLoc(),
                                  Twine("Section too large, can't encode "
                                        "r_address (") + Buffer +
                                        ") into 24 bits of scattered "
                                               "relocation entry.");
      return;
    }

    uint32_t OtherHalf = 0;
    switch (Type) {
    case MachO::PPC_RELOC_SECTDIFF:
    case MachO::PPC_RELOC_LOCAL_SECTDIFF:
      break;
    case MachO::PPC_RELOC_LO16_SECTDIFF:
    case MachO::PPC_RELOC_LO14_SECTDIFF:
      OtherHalf = (FixedValue >> 16) & 0xffff;
      // applyFixupOffset longer extracts the high part because it now assumes
      // this was already done.   It looks like this is not true for the
      // FixedValue needed with Mach-O relocs. So we need to adjust FixedValue
      // again here.
      FixedValue &= 0xffff;
      break;
    case MachO::PPC_RELOC_HA16_SECTDIFF:
      OtherHalf = FixedValue & 0xffff;
      FixedValue =
          ((FixedValue >> 16) + ((FixedValue & 0x8000) ? 1 : 0)) & 0xffff;
      break;
    case MachO::PPC_RELOC_HI16_SECTDIFF:
      OtherHalf = FixedValue & 0xffff;
      FixedValue = (FixedValue >> 16) & 0xffff;
      break;
    default:
      llvm_unreachable("Invalid PPC scattered relocation type.");
      break;
    }
    // We always need a pair here.
    // Relocations are written out in reverse order, so the PAIR comes first.
    makeScatteredRelocationInfo(MRE, OtherHalf, MachO::PPC_RELOC_PAIR, Log2Size,
                                IsPCRel, Value2);
    Writer->addRelocation(nullptr, Fragment->getParent(), MRE);
  } else {
    // At this point, we either have A+offset, a branch or a "vanilla" type.
    uint32_t OtherHalf = 0;
    bool NeedRegularPair = true;
    switch (Type) {
    case MachO::PPC_RELOC_LO16:
    case MachO::PPC_RELOC_LO14:
      OtherHalf = (FixedValue >> 16) & 0xffff;
      // see comment above.
      FixedValue &= 0xffff;
      break;
    case MachO::PPC_RELOC_HA16:
      OtherHalf = FixedValue & 0xffff;
      FixedValue =
          ((FixedValue >> 16) + ((FixedValue & 0x8000) ? 1 : 0)) & 0xffff;
      break;
    case MachO::PPC_RELOC_HI16:
      OtherHalf = FixedValue & 0xffff;
      FixedValue = (FixedValue >> 16) & 0xffff;
      break;
    default:
      assert((IsBr || Type == MachO::PPC_RELOC_VANILLA) &&
             "No subtractor and not a branch or Vanilla?");
      NeedRegularPair = false;
      break;
    }

    if (NeedRegularPair) {
      // This is the case where we have Sym+offset and the PAIR is non-
      // scattered.
      makeRelocationInfo(MRE, OtherHalf, 0 /*Index*/, IsPCRel, Log2Size,
                         false /*IsExtern*/, MachO::PPC_RELOC_PAIR);
      Writer->addRelocation(nullptr, Fragment->getParent(), MRE);
    }
  }

  // CHECKME: why not "FixedValue".
  makeScatteredRelocationInfo(MRE, FixupOffset, Type, Log2Size, IsPCRel, Value);
  Writer->addRelocation(nullptr, Fragment->getParent(), MRE);
}

/// Record a regular relocation or fail.

void PPCMachObjectWriter::recordRegularRelocation(
    MachObjectWriter *Writer, const MCAssembler &Asm, const MCAsmLayout &Layout,
    const MCFragment *Fragment, const MCFixup &Fixup, MCValue Target,
    uint64_t &FixedValue, const MCFixupKind FK) {
  const unsigned Log2Size = getFixupKindLog2Size(FK);
  const bool IsPCRel = Writer->isFixupKindPCRel(Asm, FK);
  const unsigned RelocType = getRelocType(Target, FK, IsPCRel);

  // Get the symbol data, if it exists.

  const MCSymbol *A = nullptr;
  if (Target.getSymA())
    A = &Target.getSymA()->getSymbol();

  // See <reloc.h>.
  const uint32_t FixupOffset = getFixupOffset(Layout, Fragment, Fixup);
  unsigned Index = 0; // extern = 0 && Index = 0 => R_ABS.
  unsigned Type = RelocType;

  const MCSymbol *RelSymbol = nullptr;
  if (!Target.isAbsolute()) {
    assert (A != nullptr && "missing symbol A");
    // Resolve constant variables.
    if (A->isVariable()) {
      int64_t Res;
      if (A->getVariableValue()->evaluateAsAbsolute(
              Res, Layout, Writer->getSectionAddressMap())) {
        FixedValue = Res;
        return;
      }
    }

    // Check whether we need an external or internal relocation.
    if (Writer->doesSymbolRequireExternRelocation(*A)) {
      RelSymbol = A;
      // For external relocations, make sure to offset the fixup value to
      // compensate for the addend of the symbol address, if it was
      // undefined. This occurs with weak definitions, for example.
      if (!A->isUndefined())
        FixedValue -= Layout.getSymbolOffset(*A);
    } else {
      // The index is the section ordinal (1-based).
      const MCSection &Sec = A->getSection();
      Index = Sec.getOrdinal() + 1;
      FixedValue += Writer->getSectionAddress(&Sec);
    }
    if (IsPCRel)
      FixedValue -= Writer->getSectionAddress(Fragment->getParent());
  }

  uint32_t OtherHalf = 0;
  bool NeedsPair = true;
  switch (Type) {
  default:
    NeedsPair = false;
   break;
  case MachO::PPC_RELOC_LO16:
  case MachO::PPC_RELOC_LO14:
      OtherHalf = (FixedValue >> 16) & 0xffff;
      FixedValue &= 0xffff;
      break;
  case MachO::PPC_RELOC_HA16:
      OtherHalf = FixedValue & 0xffff;
      FixedValue =
          ((FixedValue >> 16) + ((FixedValue & 0x8000) ? 1 : 0)) & 0xffff;
      break;
  case MachO::PPC_RELOC_HI16:
      OtherHalf = FixedValue & 0xffff;
      FixedValue = (FixedValue >> 16) & 0xffff;
      break;
  }

  MachO::any_relocation_info MRE;
  if (NeedsPair) {
    makeRelocationInfo(MRE, OtherHalf, 0, IsPCRel, Log2Size, false, MachO::PPC_RELOC_PAIR);
    Writer->addRelocation(RelSymbol, Fragment->getParent(), MRE);
  }
  // struct relocation_info (8 bytes)
  makeRelocationInfo(MRE, FixupOffset, Index, IsPCRel, Log2Size, false, Type);
  Writer->addRelocation(RelSymbol, Fragment->getParent(), MRE);
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createPPCMachObjectWriter(bool Is64Bit, uint32_t CPUType,
                                uint32_t CPUSubtype) {
  return llvm::make_unique<PPCMachObjectWriter>(Is64Bit, CPUType, CPUSubtype);
}
