// RUN: %clang_cc1  -triple powerpc-apple-macosx10.5 -emit-llvm %s -fno-common -o - | \
// RUN: FileCheck %s --check-prefix=CHECK-C
// RUN: %clang_cc1  -triple powerpc64-apple-macosx10.5 -emit-llvm %s -fno-common -o - | \
// RUN: FileCheck %s --check-prefix=CHECK-C

#include <stdbool.h>

#include "vectors.h"

// Actually, the vector return ABI is the same for m32/m64

v16qi v16qi_fun () { v16qi x = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}; return x;}
// CHECK-C: define <16 x i8> @v16qi_fun()
// CHECK-C: ret <16 x i8>

v8hi  v8hi_fun ()  { v8hi  x = {0,1,2,3,4,5,6,7}; return x;}
// CHECK-C: define <8 x i16> @v8hi_fun()
// CHECK-C: ret <8 x i16>

v4si  v4si_fun ()  { v4si  x = {0,0,0,1}; return x;}
// CHECK-C: define <4 x i32> @v4si_fun()
// CHECK-C: ret <4 x i32>

v4sf  v4sf_fun ()  { v4sf  x = {0.5,1.0,2.0,4.0}; return x;}
// CHECK-C: define <4 x float> @v4sf_fun()
// CHECK-C: ret <4 x float>

v2di  v2di_fun ()  { v2di  x = {0,1}; return x;}
// CHECK-C: define i128 @v2di_fun()
// CHECK-C: ret i128

v2df  v2df_fun ()  { v2df  x = {0.0,1.0}; return x;}
// CHECK-C: define i128 @v2df_fun()
// CHECK-C: ret i128
