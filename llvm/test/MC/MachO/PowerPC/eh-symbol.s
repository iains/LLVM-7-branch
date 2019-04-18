// RUN: llvm-mc -triple powerpc-darwin %s -filetype=obj -o - | llvm-nm - | FileCheck %s

// test that we don't produce foo.eh symbols in a debug_frame section.
// CHECK-NOT: _f.eh
// CHECK: T _f
// CHECK-NOT: _f.eh

	.globl	_f
_f:
	.cfi_startproc
	blr
	.cfi_endproc

	.cfi_sections .debug_frame
