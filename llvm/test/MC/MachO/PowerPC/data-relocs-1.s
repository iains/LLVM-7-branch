// RUN: llvm-mc -triple powerpc-darwin -filetype=obj %s -o - | llvm-readobj -r -s -sd | \
// RUN: FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-PPC
// RUN: llvm-mc -triple powerpc64-darwin -filetype=obj %s -o - | llvm-readobj -r -s -sd | \
// RUN: FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-PPC64

_b:	.byte 1

	.byte _a
	.byte _a+6
	.byte _a-6

# The next two are not allowed (undefined symbols in subtraction).
#	.byte _a-.
#	.byte .-_a

# This forces a relocations for the pc-symbol and symbol-pc cases, if it
# is not present, the compiler will resolve the offset as a constant.
# This is an enhancement of the LLVM-based assembler (cctools will apply
# the relocation always).
_force_reloca:

	.byte _b
	.byte _b-.
	.byte _b+6
	.byte _b-6

_d:	.short 2

	.short _c
	.short _c+6
	.short _c-6
# The next two are not allowed (undefined symbols in subtraction).
#	.short _c-.
#	.short .-_c

_force_relocb:

	.short _d
	.short _d-.
	.short _d+6
	.short _d-6

_f:	.long 3

	.long _e
	.long _e+6
	.long _e-6
# The next two are not allowed (undefined symbols in subtraction).
#	.long _e-.
#	.long _.-_e

_force_relocc:
	.long _f
	.long _f-.
	.long $-_f
	.long _f+6
	.long _f-6

# Not allowed for 32b.
#	.quad _h

 	.subsections_via_symbols

# CHECK-PPC:  Format: Mach-O 32-bit ppc
# CHECK-PPC: Arch: powerpc
# CHECK-PPC: AddressSize: 32bit
# CHECK-PPC64: Format: Mach-O 64-bit ppc64
# CHECK-PPC64: Arch: powerpc64
# CHECK-PPC64: AddressSize: 64bit
# CHECK: Sections [
# CHECK:   Section {
# CHECK:      Index: 0
# CHECK:     Name: __text (5F 5F 74 65 78 74 00 00 00 00 00 00 00 00 00 00)
# CHECK:     Segment: __TEXT (5F 5F 54 45 58 54 00 00 00 00 00 00 00 00 00 00)
# CHECK:     Address: 0x0
# CHECK:     Size: 0x3C
# CHECK-PPC:     Offset: 256
# CHECK-PPC64:     Offset: 288
# CHECK:     Alignment: 0
# CHECK-PPC:     RelocationOffset: 0x13C
# CHECK-PPC64:     RelocationOffset: 0x15C
# CHECK:     RelocationCount: 26
# CHECK:     Type: 0x0
# CHECK:     Attributes [ (0x800000)
# CHECK:       PureInstructions (0x800000)
# CHECK:     ]
# CHECK:     Reserved1: 0x0
# CHECK:     Reserved2: 0x0
# CHECK-PPC64:     Reserved3: 0x0
# CHECK:     SectionData (
# CHECK:       0000: 010006FA 00FB06FA 00020000 0006FFFA  |................|
# CHECK:       0010: 0008FFF6 000E0002 00000003 00000000  |................|
# CHECK:       0020: 00000006 FFFFFFFA 00000018 FFFFFFEC  |................|
# CHECK:       0030: 00000018 0000001E 00000012           |............|
# CHECK:     )
# CHECK:   }
# CHECK: ]
# CHECK: Relocations [
# CHECK:   Section __text {
# CHECK:     0x38 0 2 n/a PPC_RELOC_VANILLA 1 0x18
# CHECK:     0x34 0 2 n/a PPC_RELOC_VANILLA 1 0x18
# CHECK:     0x30 0 2 n/a PPC_RELOC_LOCAL_SECTDIFF 1 0x30
# CHECK:     0x0 0 2 n/a PPC_RELOC_PAIR 1 0x18
# CHECK:     0x2C 0 2 n/a PPC_RELOC_LOCAL_SECTDIFF 1 0x18
# CHECK:     0x0 0 2 n/a PPC_RELOC_PAIR 1 0x2C
# CHECK:     0x28 0 2 0 PPC_RELOC_VANILLA 0 __text
# CHECK:     0x24 0 2 1 PPC_RELOC_VANILLA 0 _e
# CHECK:     0x20 0 2 1 PPC_RELOC_VANILLA 0 _e
# CHECK:     0x1C 0 2 1 PPC_RELOC_VANILLA 0 _e
# CHECK:     0x16 0 1 n/a PPC_RELOC_VANILLA 1 0x8
# CHECK:     0x14 0 1 n/a PPC_RELOC_VANILLA 1 0x8
# CHECK:     0x12 0 1 n/a PPC_RELOC_LOCAL_SECTDIFF 1 0x8
# CHECK:     0x0 0 1 n/a PPC_RELOC_PAIR 1 0x12
# CHECK:     0x10 0 1 0 PPC_RELOC_VANILLA 0 __text
# CHECK:     0xE 0 1 1 PPC_RELOC_VANILLA 0 _c
# CHECK:     0xC 0 1 1 PPC_RELOC_VANILLA 0 _c
# CHECK:     0xA 0 1 1 PPC_RELOC_VANILLA 0 _c
# CHECK:     0x7 0 0 n/a PPC_RELOC_VANILLA 1 0x0
# CHECK:     0x6 0 0 n/a PPC_RELOC_VANILLA 1 0x0
# CHECK:     0x5 0 0 n/a PPC_RELOC_LOCAL_SECTDIFF 1 0x0
# CHECK:     0x0 0 0 n/a PPC_RELOC_PAIR 1 0x5
# CHECK:     0x4 0 0 0 PPC_RELOC_VANILLA 0 __text
# CHECK:     0x3 0 0 1 PPC_RELOC_VANILLA 0 _a
# CHECK:     0x2 0 0 1 PPC_RELOC_VANILLA 0 _a
# CHECK:     0x1 0 0 1 PPC_RELOC_VANILLA 0 _a
# CHECK:   }
# CHECK: ]
