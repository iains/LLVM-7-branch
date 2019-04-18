// RUN: llvm-mc -triple powerpc64-darwin -filetype=obj %s -o - | llvm-readobj -r -s -sd | \
// RUN: FileCheck %s --check-prefix=CHECK-PPC64

# These are only relevant for ppc64

	.globl _h
_h:	.quad 3
Local:	.quad 6

	.quad _g	; V, non-scattered
	.quad _g+7	; V, non-scattered
	.quad _g-8	; V, non-scattered

# These should not produce relocations, for LLVM (but they do for cctools).
#	.quad Local
#	.quad Local+9
#	.quad Local-10

	.quad $-11	; V, scattered
	.quad $+12	; V, scattered


# This forces a relocations for the pc-symbol and symbol-pc cases, if it
# is not present, the compiler will resolve the offset as a constant.
# This is an enhancement of the LLVM-based assembler (cctools will apply
# the relocation always).
_force_reloc:
	.quad _h	; V, non-scattered
	.quad _h-$	; secdiff, scattered
	.quad $-_h	; locsdiff, scattered
	.quad _h+13	; V, scattered
	.quad _h-14	; V, scattered

	.quad Local	; non-scattered
	.quad Local-$	; locsdiff, scattered
	.quad $-Local	; locsdiff, scattered
	.quad Local+15	; V, scattered
	.quad Local-16	; V, scattered

# Not allowed because undefined symbols and not permitted in a non-
# constant subtraction.
#	.quad _g-.
#	.quad .-_g

 	.subsections_via_symbols

# CHECK-PPC64: Format: Mach-O 64-bit ppc64
# CHECK-PPC64: Arch: powerpc64
# CHECK-PPC64: AddressSize: 64bit
# CHECK-PPC64: Sections [
# CHECK-PPC64:   Section {
# CHECK-PPC64:     Index: 0
# CHECK-PPC64:     Name: __text (5F 5F 74 65 78 74 00 00 00 00 00 00 00 00 00 00)
# CHECK-PPC64:     Segment: __TEXT (5F 5F 54 45 58 54 00 00 00 00 00 00 00 00 00 00)
# CHECK-PPC64:     Address: 0x0
# CHECK-PPC64:     Size: 0x88
# CHECK-PPC64:     Offset: 288
# CHECK-PPC64:     Alignment: 0
# CHECK-PPC64:     RelocationOffset: 0x1A8
# CHECK-PPC64:     RelocationCount: 19
# CHECK-PPC64:     Type: 0x0
# CHECK-PPC64:     Attributes [ (0x800000)
# CHECK-PPC64:       PureInstructions (0x800000)
# CHECK-PPC64:     ]
# CHECK-PPC64:     Reserved1: 0x0
# CHECK-PPC64:     Reserved2: 0x0
# CHECK-PPC64:     Reserved3: 0x0
# CHECK-PPC64:     SectionData (
# CHECK-PPC64:       0000: 00000000 00000003 00000000 00000006  |................|
# CHECK-PPC64:       0010: 00000000 00000000 00000000 00000007  |................|
# CHECK-PPC64:       0020: FFFFFFFF FFFFFFF8 00000000 0000001D  |................|
# CHECK-PPC64:       0030: 00000000 0000003C 00000000 00000000  |.......<........|
# CHECK-PPC64:       0040: FFFFFFFF FFFFFFC0 00000000 00000048  |...............H|
# CHECK-PPC64:       0050: 00000000 0000000D FFFFFFFF FFFFFFF2  |................|
# CHECK-PPC64:       0060: 00000000 00000008 FFFFFFFF FFFFFFA0  |................|
# CHECK-PPC64:       0070: 00000000 00000068 00000000 00000017  |.......h........|
# CHECK-PPC64:       0080: FFFFFFFF FFFFFFF8                    |........|
# CHECK-PPC64:     )
# CHECK-PPC64:   }
# CHECK-PPC64: ]
# CHECK-PPC64: Relocations [
# CHECK-PPC64:   Section __text {
# CHECK-PPC64:     0x80 0 3 n/a PPC_RELOC_VANILLA 1 0x8
# CHECK-PPC64:     0x78 0 3 n/a PPC_RELOC_VANILLA 1 0x8
# CHECK-PPC64:     0x70 0 3 n/a PPC_RELOC_LOCAL_SECTDIFF 1 0x70
# CHECK-PPC64:     0x0 0 3 n/a PPC_RELOC_PAIR 1 0x8
# CHECK-PPC64:     0x68 0 3 n/a PPC_RELOC_LOCAL_SECTDIFF 1 0x8
# CHECK-PPC64:     0x0 0 3 n/a PPC_RELOC_PAIR 1 0x68
# CHECK-PPC64:     0x60 0 3 0 PPC_RELOC_VANILLA 0 __text
# CHECK-PPC64:     0x58 0 3 n/a PPC_RELOC_VANILLA 1 0x0
# CHECK-PPC64:     0x50 0 3 n/a PPC_RELOC_VANILLA 1 0x0
# CHECK-PPC64:     0x48 0 3 n/a PPC_RELOC_LOCAL_SECTDIFF 1 0x48
# CHECK-PPC64:     0x0 0 3 n/a PPC_RELOC_PAIR 1 0x0
# CHECK-PPC64:     0x40 0 3 n/a PPC_RELOC_SECTDIFF 1 0x0
# CHECK-PPC64:     0x0 0 3 n/a PPC_RELOC_PAIR 1 0x40
# CHECK-PPC64:     0x38 0 3 0 PPC_RELOC_VANILLA 0 __text
# CHECK-PPC64:     0x30 0 3 n/a PPC_RELOC_VANILLA 1 0x30
# CHECK-PPC64:     0x28 0 3 n/a PPC_RELOC_VANILLA 1 0x28
# CHECK-PPC64:     0x20 0 3 1 PPC_RELOC_VANILLA 0 _g
# CHECK-PPC64:     0x18 0 3 1 PPC_RELOC_VANILLA 0 _g
# CHECK-PPC64:     0x10 0 3 1 PPC_RELOC_VANILLA 0 _g
# CHECK-PPC64:   }
# CHECK-PPC64: ]
