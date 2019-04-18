// RUN: llvm-mc < %s -triple powerpc-darwin -filetype=obj | llvm-readobj -r |\
// RUN: FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-PPC
// RUN: llvm-mc < %s -triple powerpc64-darwin -filetype=obj | llvm-readobj -r |\
// RUN: FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-PPC64


# x86 no longer uses relocations for EH frames, but (for now) to be backward-
# compatible with older tools, we will continue to use them for ppc/ppc64.

	.globl	_bar
	.align	4
_bar:
	.cfi_startproc
	.cfi_endproc

// CHECK:      Relocations [
// CHECK:  Section __eh_frame {
// CHECK-PPC:    0x1C 0 2 n/a PPC_RELOC_LOCAL_SECTDIFF 1 0x0
// CHECK-PPC:    0x0 0 2 n/a PPC_RELOC_PAIR 1 0x1C
// CHECK-PPC64:    0x1C 0 3 n/a PPC_RELOC_LOCAL_SECTDIFF 1 0x0
// CHECK-PPC64:    0x0 0 3 n/a PPC_RELOC_PAIR 1 0x1C
// CHECK:  }
// CHECK: ]
