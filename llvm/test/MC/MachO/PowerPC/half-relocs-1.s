# RUN: llvm-mc -triple powerpc-darwin -filetype=obj %s -o - | llvm-readobj -r -s -sd | \
# RUN: FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-PPC

	.text
#PIC
_fun:
	bcl 20, 31, Lx
Lx:
	mflr r10
        addis r2,r10,ha16(L_x$non_lazy_ptr-Lx)
        lwz r2,lo16(L_x$non_lazy_ptr-Lx)(r2)
        lwz r3,0(r2)
        blr
        
# dynamic-no-pic
_fun1:
        lis r2,ha16(L_x$non_lazy_ptr)
        lwz r2,lo16(L_x$non_lazy_ptr)(r2)
        lwz r3,0(r2)
        blr


	.comm _x,4,2
        .non_lazy_symbol_pointer
L_x$non_lazy_ptr:
        .indirect_symbol _x
        .long   0
        .subsections_via_symbols

# CHECK-PPC: Format: Mach-O 32-bit ppc
# CHECK-PPC: Arch: powerpc
# CHECK-PPC: AddressSize: 32bit
# CHECK: Sections [
# CHECK:   Section {
# CHECK:     Index: 0
# CHECK:     Name: __text (5F 5F 74 65 78 74 00 00 00 00 00 00 00 00 00 00)
# CHECK:     Segment: __TEXT (5F 5F 54 45 58 54 00 00 00 00 00 00 00 00 00 00)
# CHECK:     Address: 0x0
# CHECK-PPC:     Size: 0x28
# CHECK-PPC:     Offset: 324
# CHECK:     Alignment: 0
# CHECK-PPC:     RelocationOffset: 0x170
# CHECK:     RelocationCount: 8
# CHECK:     Type: 0x0
# CHECK:     Attributes [ (0x800004)
# CHECK:       PureInstructions (0x800000)
# CHECK:       SomeInstructions (0x4)
# CHECK:     ]
# CHECK:     Reserved1: 0x0
# CHECK:     Reserved2: 0x0
# CHECK:     SectionData (
# CHECK:       0000: 429F0005 7D4802A6 3C4A0000 80420024  |B...}H..<J...B.$|
# CHECK:       0010: 80620000 4E800020 3C400000 80420028  |.b..N.. <@...B.(|
# CHECK:       0020: 80620000 4E800020                    |.b..N.. |
# CHECK:     )
# CHECK:   }
# CHECK:   Section {
# CHECK:     Index: 1
# CHECK:     Name: __nl_symbol_ptr (5F 5F 6E 6C 5F 73 79 6D 62 6F 6C 5F 70 74 72 00)
# CHECK:     Segment: __DATA (5F 5F 44 41 54 41 00 00 00 00 00 00 00 00 00 00)
# CHECK:     Address: 0x28
# CHECK-PPC:     Size: 0x4
# CHECK-PPC:     Offset: 364
# CHECK:     Alignment: 2
# CHECK:     RelocationOffset: 0x0
# CHECK:     RelocationCount: 0
# CHECK:     Type: 0x6
# CHECK:     Attributes [ (0x0)
# CHECK:     ]
# CHECK:     Reserved1: 0x0
# CHECK:     Reserved2: 0x0
# CHECK:     SectionData (
# CHECK:       0000: 00000000                             |....|
# CHECK:     )
# CHECK:   }
# CHECK: ]
# CHECK: Relocations [
# CHECK:   Section __text {
# CHECK:     0x1C 0 2 0 PPC_RELOC_LO16 0 __nl_symbol_ptr
# CHECK:     0x0 0 2 0 PPC_RELOC_PAIR 0 -
# CHECK:     0x18 0 2 0 PPC_RELOC_HA16 0 __nl_symbol_ptr
# CHECK:     0x28 0 2 0 PPC_RELOC_PAIR 0 -
# CHECK:     0xC 0 2 n/a PPC_RELOC_LO16_SECTDIFF 1 0x28
# CHECK:     0x0 0 2 n/a PPC_RELOC_PAIR 1 0x4
# CHECK:     0x8 0 2 n/a PPC_RELOC_HA16_SECTDIFF 1 0x28
# CHECK:     0x24 0 2 n/a PPC_RELOC_PAIR 1 0x4
# CHECK:   }
# CHECK: ]
