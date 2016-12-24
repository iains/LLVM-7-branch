//===-- PPCMcpu.h - Define recognised CPU variants for PPC ----*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the PowerPC CPU models recognised by the backend.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_POWERPC_PPCMCPU_H
#define LLVM_LIB_TARGET_POWERPC_PPCMCPU_H

// GCC #defines PPC on Linux but we use it as our namespace name
#undef PPC

namespace llvm {
class StringRef;

/// This needs to stay in sync with the enum in PPCMcpu.cpp.

namespace PPC {
  enum {
    MCPU_NONE,
    MCPU_32,
    MCPU_440,
    MCPU_601,
    MCPU_602,
    MCPU_603,
    MCPU_750,
    MCPU_7400,
    MCPU_7450,
    MCPU_970,
    MCPU_E500,
    MCPU_E500mc,
    MCPU_E5500,
    MCPU_A2,
    MCPU_PWR3,
    MCPU_PWR4,
    MCPU_PWR5,
    MCPU_PWR5X,
    MCPU_PWR6,
    MCPU_PWR6X,
    MCPU_PWR7,
    MCPU_PWR8,
    MCPU_PWR9,
    MCPU_64,
    MCPU_64LE,
    MCPU_ANY,
    MCPU_BAD // must be last
  };

StringRef mcpuStringFromEnum(unsigned MCPU);

unsigned mcpuEnumFromString(StringRef CPUName);

/// MachO CPU SubType for file headers.
unsigned cpuSubTypeFromString(StringRef CPUName);

} // PPC

} // llvm
#endif // LLVM_LIB_TARGET_POWERPC_PPCMCPU_H
