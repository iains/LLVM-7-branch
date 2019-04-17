// RUN: %clang_cc1  -triple powerpc-apple-macosx10.5 -emit-llvm %s -fno-common -o - | \
// RUN: FileCheck %s --check-prefix=CHECK-C --check-prefix=CHECK-32
// RUN: %clang_cc1  -triple powerpc64-apple-macosx10.5 -emit-llvm %s -fno-common -o - | \
// RUN: FileCheck %s --check-prefix=CHECK-C --check-prefix=CHECK-64

// CHECK-32: powerpc-apple-macosx10.5
// CHECK-64: powerpc64-apple-macosx10.5

_Complex char        c_fun (void) { _Complex char         x = 2+1i   ; return x; }
// CHECK-C: define i16 @c_fun()
// CHECK-C: ret i16

_Complex short       s_fun (void) { _Complex short       x = 2+1i   ; return x; }
// CHECK-C: define i32 @s_fun()
// CHECK-C: ret i32

_Complex int         i_fun (void) { _Complex int         x = 2+1i   ; return x; }
// CHECK-C: define i64 @i_fun()
// CHECK-C: ret i64

_Complex long        l_fun (void) { _Complex long        x = 2L+1Li  ; return x; }
// CHECK-32: define i64 @l_fun()
// CHECK-32: ret i64
// CHECK-64: define i128 @l_fun()
// CHECK-64: ret i128

_Complex long long   L_fun (void) { _Complex long long   x = 2LL+1LLi ; return x; }
// CHECK-C: define i128 @L_fun()
// CHECK-C: ret i128

_Complex float       f_fun (void) { _Complex float       x = 2.0F+1.0Fi; return x; }
// CHECK-C: define i64 @f_fun()
// CHECK-C: ret i64

_Complex double      d_fun (void) { _Complex double      x = 2.0+1.0i ; return x; }
// CHECK-C: define i128 @d_fun()
// CHECK-C: ret i128

_Complex long double D_fun (void) { _Complex long double x = 2.0L+1.0Li; return x; }
// CHECK-C: define i256 @D_fun()
// CHECK-C: ret i256
