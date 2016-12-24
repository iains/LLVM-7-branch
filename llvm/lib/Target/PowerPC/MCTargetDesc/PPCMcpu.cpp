//===-- PPCMcpu.cpp - mcpu and machine directive string utils -*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implement some utility functions to convert mcpu strings to and from the
// relevant enumerations.
//
//===----------------------------------------------------------------------===//

#include "PPCMcpu.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/BinaryFormat/MachO.h"

namespace llvm {

/// All of this needs to stay in sync with the enum in PPCMcpu.h.

namespace PPC {

static const char *const CPUNames[] = {
    "",
    "ppc",
    "440",
    "601",
    "602",
    "603",
    "750",
    "7400",
    "7450",
    "970",
    "e500mc",
    "e5500",
    "a2",
    "pwr3",
    "pwr4",
    "pwr5",
    "pwr5x",
    "pwr6",
    "pwr6x",
    "pwr7",
    "pwr8",
    "pwr9",
    "ppc64",
    "ppc64le",
    "any"
};

/// When -mcpu is used on the command line, it uses the TableGen'd code to match
/// the value to a CPU, outside of that case, it's sometimes necessary to
/// convert the enum back to the string.
StringRef mcpuStringFromEnum(unsigned MCPU) {
  assert(MCPU < array_lengthof(CPUNames) &&
         "CPUNames[] might not be up-to-date!");
  return CPUNames[MCPU];
}

/// This equates the CPU name as string to the enum.
/// It caters for both the regular ELF case e.g. "7400" and the MachO version
/// e.g. ppc7400, this allows us to parse legacy MachO code, but use the
/// same as ELF going forward.

unsigned mcpuEnumFromString(StringRef CPUName) {
  unsigned Mcpu = StringSwitch<unsigned>(CPUName)
   .Case(       "", MCPU_NONE)
   .Case(    "ppc", MCPU_32)
   .Case(    "440", MCPU_440)
   .Cases(   "601",  "ppc601", MCPU_601)
   .Cases(   "602",  "ppc602", MCPU_602)
   .Cases(   "603",  "ppc603", MCPU_603)
   .Cases(   "750",  "ppc750", MCPU_750)
   .Cases(  "7400", "ppc7400", MCPU_7400)
   .Cases(  "7450", "ppc7450", MCPU_7450)
   .Cases(   "970",  "ppc970", MCPU_970)
   .Case( "e500mc", MCPU_E500mc)
   .Case(  "e5500", MCPU_E5500)
   .Case(     "a2", MCPU_A2)
   .Case(   "pwr3", MCPU_PWR3)
   .Case(   "pwr4", MCPU_PWR4)
   .Case(   "pwr5", MCPU_PWR5)
   .Case(  "pwr5x", MCPU_PWR5X)
   .Case(   "pwr6", MCPU_PWR6)
   .Case(  "pwr6x", MCPU_PWR6X)
   .Case(   "pwr7", MCPU_PWR7)
   .Case(   "pwr8", MCPU_PWR8)
   .Case(   "pwr9", MCPU_PWR9)
   .Case(  "ppc64", MCPU_64)
   .Case("ppc64le", MCPU_64LE)
   .Case(    "any", MCPU_ANY)
   .Default(MCPU_BAD);
   return Mcpu;
}

// Possible values, not all currently implemented.
// CPU_SUBTYPE_POWERPC_ALL
// CPU_SUBTYPE_POWERPC_601
// CPU_SUBTYPE_POWERPC_602
// CPU_SUBTYPE_POWERPC_603
// CPU_SUBTYPE_POWERPC_603e
// CPU_SUBTYPE_POWERPC_603ev
// CPU_SUBTYPE_POWERPC_604
// CPU_SUBTYPE_POWERPC_604e
// CPU_SUBTYPE_POWERPC_620
// CPU_SUBTYPE_POWERPC_750
// CPU_SUBTYPE_POWERPC_7400
// CPU_SUBTYPE_POWERPC_7450
// CPU_SUBTYPE_POWERPC_970

unsigned cpuSubTypeFromString(StringRef CPUName) {
  unsigned Mcpu = StringSwitch<unsigned>(CPUName)
   .Case(       "", MachO::CPU_SUBTYPE_POWERPC_ALL)
   .Case(    "ppc", MachO::CPU_SUBTYPE_POWERPC_ALL)
   .Cases(   "601",  "ppc601", MachO::CPU_SUBTYPE_POWERPC_601)
   .Cases(   "602",  "ppc602", MachO::CPU_SUBTYPE_POWERPC_602)
   .Cases(   "603",  "ppc603", MachO::CPU_SUBTYPE_POWERPC_603)
   .Cases(   "750",  "ppc750", MachO::CPU_SUBTYPE_POWERPC_750)
   .Cases(  "7400", "ppc7400", MachO::CPU_SUBTYPE_POWERPC_7400)
   .Cases(  "7450", "ppc7450", MachO::CPU_SUBTYPE_POWERPC_7450)
   .Cases(   "970",  "ppc970", MachO::CPU_SUBTYPE_POWERPC_970)
   .Case(  "ppc64", MachO::CPU_SUBTYPE_POWERPC_ALL)
   .Case(    "all", MachO::CPU_SUBTYPE_POWERPC_ALL)
   .Case(    "any", MachO::CPU_SUBTYPE_POWERPC_ALL)
   .Default(MachO::CPU_SUBTYPE_POWERPC_ALL);
   return Mcpu;


}

} // PPC
} // llvm
