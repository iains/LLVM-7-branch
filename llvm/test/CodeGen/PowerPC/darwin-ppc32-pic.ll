; RUN: llc < %s -mtriple=powerpc-apple-darwin9 -relocation-model=pic | FileCheck %s
@foobar = common global i32 0, align 4

define i32 @foo() {
entry:
  %0 = load i32, i32* @foobar, align 4
  ret i32 %0
}

; CHECK_LABEL: _foo
; CHECK: bcl 20, 31, [[PB:L[0-9]+\$pb]]
; CHECK-NEXT: [[PB]]:
; CHECK-NEXT: mflr [[PREG:r[0-9]+]]
; CHECK-NEXT: addis [[PREG]], [[PREG]], ha16(L_foobar$non_lazy_ptr-[[PB]])
; CHECK-NEXT: lwz [[REG:r[0-9]+]], lo16(L_foobar$non_lazy_ptr-[[PB]])([[PREG]])
; CHECK-NEXT: lwz r3, 0([[REG]])

; CHECK_LABEL: L_foobar$non_lazy_ptr
; CHECK: .indirect_symbol _foobar

; CHECK: .comm _foobar,4,2
