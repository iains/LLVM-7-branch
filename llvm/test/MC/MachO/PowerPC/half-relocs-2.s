# RUN: llvm-mc -triple powerpc-darwin -filetype=obj %s -o - | llvm-readobj -r -s -sd | \
# RUN: FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-PPC
# RUN: llvm-mc -triple powerpc64-darwin -filetype=obj %s -o - | llvm-readobj -r -s -sd | \
# RUN: FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-PPC64

# corresponding to -fno-common, data accesses.

	.text
#PIC
_fun:
	bcl 20, 31, Lx
Lx:
	mflr r10
        addis r2,r10,ha16(_x-Lx)
        lwz r2,lo16(_x-Lx)(r2)
# Not sure if any compiler will currently generate this, but it"s
# supposed to work.
        addis r2,r10,ha16(_x-Lx+4)
        lwz r2,lo16(_x-Lx+4)(r2)
        lwz r3,0(r2)
        blr
        
# dynamic-no-pic
_fun1:
        lis r2,ha16(_x)
        lwz r2,lo16(_x)(r2)
        lis r2,ha16(_x+4)
        lwz r2,lo16(_x+4)(r2)
        lwz r3,0(r2)
        blr

	.globl _x
	.data
_x:
	.long 0
	.long 1

        .subsections_via_symbols

# CHECK-PPC: Format: Mach-O 32-bit ppc
# CHECK-PPC: Arch: powerpc
# CHECK-PPC: AddressSize: 32bit
# CHECK-PPC64: Format: Mach-O 64-bit ppc64
# CHECK-PPC64: Arch: powerpc
# CHECK-PPC64: AddressSize: 64bit
# CHECK: Sections [
# CHECK:  Section {
# CHECK:    Index: 0
# CHECK:    Name: __text (5F 5F 74 65 78 74 00 00 00 00 00 00 00 00 00 00)
# CHECK:    Segment: __TEXT (5F 5F 54 45 58 54 00 00 00 00 00 00 00 00 00 00)
# CHECK:    Address: 0x0
# CHECK:    Size: 0x38
# CHECK-PPC:    Offset: 324
# CHECK-PPC64:     Offset: 368
# CHECK:    Alignment: 0
# CHECK-PPC:    RelocationOffset: 0x184
# CHECK-PPC64:    RelocationOffset: 0x1B0
# CHECK:    RelocationCount: 16
# CHECK:    Type: 0x0
# CHECK:    Attributes [ (0x800004)
# CHECK:      PureInstructions (0x800000)
# CHECK:      SomeInstructions (0x4)
# CHECK:    ]
# CHECK:    Reserved1: 0x0
# CHECK:    Reserved2: 0x0
# CHECK:    SectionData (
# CHECK:      0000: 429F0005 7D4802A6 3C4A0000 80420034  |B...}H..<J...B.4|
# CHECK:      0010: 3C4A0000 80420038 80620000 4E800020  |<J...B.8.b..N.. |
# CHECK:      0020: 3C400000 80420038 3C400000 8042003C  |<@...B.8<@...B.<|
# CHECK:      0030: 80620000 4E800020                    |.b..N.. |
# CHECK:    )
# CHECK:  }
# CHECK:  Section {
# CHECK:    Index: 1
# CHECK:    Name: __data (5F 5F 64 61 74 61 00 00 00 00 00 00 00 00 00 00)
# CHECK:    Segment: __DATA (5F 5F 44 41 54 41 00 00 00 00 00 00 00 00 00 00)
# CHECK:    Address: 0x38
# CHECK:    Size: 0x8
# CHECK-PPC:    Offset: 380
# CHECK-PPC64:     Offset: 424
# CHECK:    Alignment: 0
# CHECK:    RelocationOffset: 0x0
# CHECK:    RelocationCount: 0
# CHECK:    Type: 0x0
# CHECK:    Attributes [ (0x0)
# CHECK:    ]
# CHECK:    Reserved1: 0x0
# CHECK:    Reserved2: 0x0
# CHECK:    SectionData (
# CHECK:      0000: 00000000 00000001                    |........|
# CHECK:    )
# CHECK:  }
# CHECK:]
# CHECK:Relocations [
# CHECK:  Section __text {
# CHECK:    0x2C 0 2 n/a PPC_RELOC_LO16 1 0x38
# CHECK:    0x0 0 2 0 PPC_RELOC_PAIR 0 -
# CHECK:    0x28 0 2 n/a PPC_RELOC_HA16 1 0x38
# CHECK:    0x3C 0 2 0 PPC_RELOC_PAIR 0 -
# CHECK:    0x24 0 2 0 PPC_RELOC_LO16 0 __data
# CHECK:    0x0 0 2 0 PPC_RELOC_PAIR 0 -
# CHECK:    0x20 0 2 0 PPC_RELOC_HA16 0 __data
# CHECK:    0x38 0 2 0 PPC_RELOC_PAIR 0 -
# CHECK:    0x14 0 2 n/a PPC_RELOC_LO16_SECTDIFF 1 0x38
# CHECK:    0x0 0 2 n/a PPC_RELOC_PAIR 1 0x4
# CHECK:    0x10 0 2 n/a PPC_RELOC_HA16_SECTDIFF 1 0x38
# CHECK:    0x38 0 2 n/a PPC_RELOC_PAIR 1 0x4
# CHECK:    0xC 0 2 n/a PPC_RELOC_LO16_SECTDIFF 1 0x38
# CHECK:    0x0 0 2 n/a PPC_RELOC_PAIR 1 0x4
# CHECK:    0x8 0 2 n/a PPC_RELOC_HA16_SECTDIFF 1 0x38
# CHECK:    0x34 0 2 n/a PPC_RELOC_PAIR 1 0x4
# CHECK:  }
# CHECK:]
