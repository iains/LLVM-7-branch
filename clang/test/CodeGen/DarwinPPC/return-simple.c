// RUN: %clang_cc1  -triple powerpc-apple-macosx10.5 -emit-llvm %s -fno-common -o - | \
// RUN: FileCheck %s --check-prefix=CHECK-C --check-prefix=CHECK-32
// RUN: %clang_cc1  -triple powerpc64-apple-macosx10.5 -emit-llvm %s -fno-common -o - | \
// RUN: FileCheck %s --check-prefix=CHECK-C --check-prefix=CHECK-64

#include <stdbool.h>

void      v_fun (void) { }
// CHECK-C: define void @v_fun()
// CHECK-C: ret void

bool        b_fun (void) { bool        x = true   ; return x; }
// CHECK-C: define i1 @b_fun()
// CHECK-32: %{{.*}} = trunc i32 %{{.*}} to i1
// CHECK-64: %{{.*}} = trunc i8 %{{.*}} to i1
// CHECK-C: ret i1 %{{.*}}

char        c_fun (void) { char        x = 1   ; return x; }
// CHECK-C: define signext i8 @c_fun()
// CHECK-C: ret i8

short       s_fun (void) { short       x = 1   ; return x; }
// CHECK-C: define signext i16 @s_fun()
// CHECK-C: ret i16

int         i_fun (void) { int         x = 1   ; return x; }
// CHECK-32: define i32 @i_fun()
// CHECK-64: define signext i32 @i_fun()
// CHECK-C: ret i32

long        l_fun (void) { long        x = 1L  ; return x; }
// CHECK-32: define i32 @l_fun()
// CHECK-32: ret i32
// CHECK-64: define i64 @l_fun()
// CHECK-64: ret i64

long long   L_fun (void) { long long   x = 1LL ; return x; }
// CHECK-C: define i64 @L_fun()
// CHECK-C: ret i64

float       f_fun (void) { float       x = 1.0F; return x; }
// CHECK-C: define float @f_fun()
// CHECK-C: ret float

double      d_fun (void) { double      x = 1.0 ; return x; }
// CHECK-C: define double @d_fun()
// CHECK-C: ret double

long double D_fun (void) { long double x = 1.0L; return x; }
// CHECK-C: define  ppc_fp128 @D_fun()
// CHECK-C: ret ppc_fp128

unsigned char      uc_fun (void) { unsigned char      x = 1U; return x; }
// CHECK-C: define zeroext i8 @uc_fun()
// CHECK-C: ret i8

unsigned short     us_fun (void) { unsigned short     x = 1U; return x; }
// CHECK-C: define zeroext i16 @us_fun()
// CHECK-C: ret i16

unsigned int       ui_fun (void) { unsigned int       x = 1U; return x; }
// CHECK-32: define i32 @ui_fun()
// CHECK-64: define zeroext i32 @ui_fun()
// CHECK-C: ret i32

unsigned long      ul_fun (void) { unsigned long      x = 1UL; return x; }
// CHECK-32: define i32 @ul_fun()
// CHECK-32: ret i32
// CHECK-64: define i64 @ul_fun()
// CHECK-64: ret i64

unsigned long long uL_fun (void) { unsigned long long x = 1ULL; return x; }
// CHECK-C: define i64 @uL_fun()
// CHECK-C: ret i64
