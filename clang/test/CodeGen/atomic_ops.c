// XFAIL: hexagon,sparc
//        (due to not having native load atomic support)
// RUN: %clang_cc1 -triple x86_64 -emit-llvm %s -o - | FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-N
// RUN: %clang_cc1 -triple i686 -emit-llvm %s -o - | FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-N
// RUN: %clang_cc1 -triple arm64 -emit-llvm %s -o - | FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-N
// RUN: %clang_cc1 -triple mips-linux-gnu -emit-llvm %s -o - | FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-N
// Darwin (m32) has a 32bit Bool.
// RUN: %clang_cc1 -triple powerpc-apple-darwin -emit-llvm %s -o - | FileCheck %s --check-prefix=CHECK --check-prefix=CHECK-D32

void foo(int x)
{
  _Atomic(int) i = 0;
  _Atomic(short) j = 0;
  // Check that multiply / divides on atomics produce a cmpxchg loop
  i *= 2;
  // CHECK: mul nsw i32
  // CHECK: {{(cmpxchg i32*|i1 @__atomic_compare_exchange\(i32 4,)}}
  i /= 2;
  // CHECK: sdiv i32
  // CHECK: {{(cmpxchg i32*|i1 @__atomic_compare_exchange\(i32 4, )}}
  j /= x;
  // CHECK: sdiv i32
  // CHECK: {{(cmpxchg i16*|i1 @__atomic_compare_exchange\(i32 2, )}}

}

extern _Atomic _Bool b;

_Bool bar() {
// CHECK-LABEL: @bar
// CHECK-N: %[[load:.*]] = load atomic i8, i8* @b seq_cst
// CHECK-D32: %[[load:.*]] = load atomic i32, i32* @b seq_cst
// CHECK-N: %[[tobool:.*]] = trunc i8 %[[load]] to i1
// CHECK-D32: %[[tobool:.*]] = trunc i32 %[[load]] to i1
// CHECK: ret i1 %[[tobool]]
  return b;
}

extern _Atomic(_Complex int) x;

void baz(int y) {
// CHECK-LABEL: @baz
// CHECK: {{store atomic|call void @__atomic_store}}
  x += y;
}
