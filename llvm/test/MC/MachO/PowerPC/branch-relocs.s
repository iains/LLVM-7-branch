// RUN: llvm-mc -triple powerpc-darwin -filetype=obj %s -o - | llvm-readobj -r -s -sd | \
// RUN: FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-PPC
// RUN: llvm-mc -triple powerpc64-darwin -filetype=obj %s -o - | llvm-readobj -r -s -sd | \
// RUN: FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-PPC64

# Test that we generate the correct relocation entries for branches.

	.text

# extern
	b _external_undefined

	.globl _see_me
_see_me:
	nop
# A local target.
Lcross_target:

# local to section
	b _bar
	b _baz
	b Lfoo
# one that needs no reloc.
	b Lcross_target

# between sections
	b _bar1
	b _baz1
	b Lfoo1

# common pic-base load.
	bcl 20, 31, Lpicbase
Lpicbase:
	nop

_visible:

# Branch across a possible atom break (FIXME: should have a reloc).
	b Lcross_target
	b _see_me

# these produce scattered relocs.
	b _table+8
	bcl 20, 31, _table+4

# Special case for direct objc method call (absolute value).
#	b 0x123454 	

Lfoo:
	blr

	.globl _bar
_bar:
	blr

	.private_extern _baz
_baz:
	blr

	.section __TEXT,__another_sect
	.globl _bar1
_bar1:
	blr

	.private_extern _baz1
_baz1:
	blr

Lfoo1:
	blr

# Some of the target assembler uses tables of function entry points accessed as
# _extern_sym + offset.

_table:
	nop
	nop
	nop
	nop
	blr

	.subsections_via_symbols

# CHECK-PPC: Format: Mach-O 32-bit ppc
# CHECK-PPC: Arch: powerpc
# CHECK-PPC: AddressSize: 32bit
# CHECK-PPC64: Format: Mach-O 64-bit ppc64
# CHECK-PPC64: Arch: powerpc64
# CHECK-PPC64: AddressSize: 64bit
# CHECK: Sections [
# CHECK:   Section {
# CHECK:     Index: 0
# CHECK:     Name: __text (5F 5F 74 65 78 74 00 00 00 00 00 00 00 00 00 00)
# CHECK:     Segment: __TEXT (5F 5F 54 45 58 54 00 00 00 00 00 00 00 00 00 00)
# CHECK:     Address: 0x0
# CHECK:     Size: 0x48
# CHECK-PPC:     Offset: 324
# CHECK-PPC64:     Offset: 368
# CHECK:     Alignment: 0
# CHECK-PPC:     RelocationOffset: 0x1AC
# CHECK-PPC64:     RelocationOffset: 0x1D8
# CHECK:     RelocationCount: 9
# CHECK:     Type: 0x0
# CHECK:     Attributes [ (0x800004)
# CHECK:       PureInstructions (0x800000)
# CHECK:       SomeInstructions (0x4)
# CHECK:     ]
# CHECK:     Reserved1: 0x0
# CHECK:     Reserved2: 0x0
# CHECK-PPC64:     Reserved3: 0x0
# CHECK:     SectionData (
# CHECK:      0000: 48000000 60000000 48000038 48000038  |H...`...H..8H..8|
# CHECK:      0010: 4800002C 4BFFFFF4 48000030 48000030  |H..,K...H..0H..0|
# CHECK:      0020: 48000030 429F0005 60000000 4BFFFFDC  |H..0B...`...K...|
# CHECK:      0030: 4BFFFFD4 48000028 429F0021 4E800020  |K...H..(B..!N.. |
# CHECK:      0040: 4E800020 4E800020                    |N.. N.. |
# CHECK:     )
# CHECK:   }
# CHECK:   Section {
# CHECK:     Index: 1
# CHECK:     Name: __another_sect (5F 5F 61 6E 6F 74 68 65 72 5F 73 65 63 74 00 00)
# CHECK:     Segment: __TEXT (5F 5F 54 45 58 54 00 00 00 00 00 00 00 00 00 00)
# CHECK:     Address: 0x48
# CHECK:     Size: 0x20
# CHECK-PPC:     Offset: 396
# CHECK-PPC64:     Offset: 440
# CHECK:     Alignment: 0
# CHECK:     RelocationOffset: 0x0
# CHECK:     RelocationCount: 0
# CHECK:     Type: 0x0
# CHECK:     Attributes [ (0x4)
# CHECK:       SomeInstructions (0x4)
# CHECK:     ]
# CHECK:     Reserved1: 0x0
# CHECK:     Reserved2: 0x0
# CHECK-PPC64:     Reserved3: 0x0
# CHECK:     SectionData (
# CHECK:       0000: 4E800020 4E800020 4E800020 60000000  |N.. N.. N.. `...|
# CHECK:       0010: 60000000 60000000 60000000 4E800020  |`...`...`...N.. |
# CHECK:     )
# CHECK:   }
# CHECK: ]
# CHECK: Relocations [
# CHECK:   Section __text {
# CHECK:    0x38 1 2 n/a PPC_RELOC_BR14 1 0x54
# CHECK:    0x34 1 2 n/a PPC_RELOC_BR24 1 0x54
# CHECK:    0x30 1 2 0 PPC_RELOC_BR24 0 __text
# CHECK:    0x20 1 2 0 PPC_RELOC_BR24 0 __another_sect
# CHECK:    0x1C 1 2 0 PPC_RELOC_BR24 0 __another_sect
# CHECK:    0x18 1 2 0 PPC_RELOC_BR24 0 __another_sect
# CHECK:    0xC 1 2 0 PPC_RELOC_BR24 0 __text
# CHECK:    0x8 1 2 0 PPC_RELOC_BR24 0 __text
# CHECK:    0x0 1 2 1 PPC_RELOC_BR24 0 _external_undefined
# CHECK-PPC:   }
# CHECK: ]
