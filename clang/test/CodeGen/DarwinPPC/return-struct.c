// RUN: %clang_cc1  -triple powerpc-apple-macosx10.5 -emit-llvm %s -fno-common -o - | \
// RUN: FileCheck %s --check-prefix=CHECK-C --check-prefix=CHECK-32
// RUN: %clang_cc1  -triple powerpc64-apple-macosx10.5 -emit-llvm %s -fno-common -o - | \
// RUN: FileCheck %s --check-prefix=CHECK-C --check-prefix=CHECK-64

#include "vectors.h"
#include "structs.h"

// We allocate the sret for empty structures.
struct _e  e_fun (void) { struct _e e; return e;}
// CHECK-C: define void @e_fun(%struct._e* inreg noalias sret %agg.result)
// CHECK-C: ret void

// We allocate the sret for empty structures.
struct _e  x_fun (int x) { struct _e e; return e;}
// CHECK-32: define void @x_fun(%struct._e* inreg noalias sret %agg.result, i32 %x)
// CHECK-64: define void @x_fun(%struct._e* inreg noalias sret %agg.result, i32 signext %x)
// CHECK-C: ret void

// Otherwise, for small structures, we return as a direct bitcast to intxxx for m64
// and via sret for m32.

struct _b  b_fun (void) { struct _b b = {true}; return b;}
// CHECK-32: define void @b_fun(%struct._b* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define %struct._b @b_fun()
// CHECK-64: ret %struct._b

struct _c  c_fun (void) { struct _c c = {'a'}; return c;}
// CHECK-32: define void @c_fun(%struct._c* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define %struct._c @c_fun()
// CHECK-64: ret %struct._c

struct _s s_fun (void) { struct _s s = {1}; return s;}
// CHECK-32: define void @s_fun(%struct._s* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define %struct._s @s_fun()
// CHECK-64: ret %struct._s

struct _i i_fun (void) { struct _i i = {1}; return i;}
// CHECK-32: define void @i_fun(%struct._i* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define %struct._i @i_fun()
// CHECK-64: ret %struct._i

struct _l l_fun (void) { struct _l l = {1L}; return l;}
// CHECK-32: define void @l_fun(%struct._l* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define %struct._l @l_fun()
// CHECK-64: ret %struct._l

struct _L L_fun (void) { struct _L L = {1L}; return L;}
// CHECK-32: define void @L_fun(%struct._L* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define %struct._L @L_fun()
// CHECK-64: ret %struct._L

struct _f f_fun (void) { struct _f f = {1.0F}; return f;}
// CHECK-32: define void @f_fun(%struct._f* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define %struct._f @f_fun()
// CHECK-64: ret %struct._f

struct _d d_fun (void) { struct _d d = {1.0}; return d;}
// CHECK-32: define void @d_fun(%struct._d* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define %struct._d @d_fun()
// CHECK-64: ret %struct._d

struct _D D_fun (void) { struct _D D = {1.0L}; return D;}
// CHECK-32: define void @D_fun(%struct._D* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define i128 @D_fun()
// CHECK-64: ret i128

struct _v v_fun (void) { struct _v v = {{0}}; return v;}
// CHECK-32: define void @v_fun(%struct._v* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define i128 @v_fun()
// CHECK-64: ret i128

struct _bb  bb_fun (void) { struct _bb bb = {true, false}; return bb;}
// CHECK-32: define void @bb_fun(%struct._bb* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define %struct._bb @bb_fun()
// CHECK-64: ret %struct._bb

struct _cc  cc_fun (void) { struct _cc cc = {'a', 'b'}; return cc;}
// CHECK-32: define void @cc_fun(%struct._cc* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define %struct._cc @cc_fun()
// CHECK-64: ret %struct._cc

struct _ss ss_fun (void) { struct _ss ss = {1,0}; return ss;}
// CHECK-32: define void @ss_fun(%struct._ss* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define %struct._ss @ss_fun()
// CHECK-64: ret %struct._ss

struct _ii ii_fun (void) { struct _ii ii = {1,2}; return ii;}
// CHECK-32: define void @ii_fun(%struct._ii* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define %struct._ii @ii_fun()
// CHECK-64: ret %struct._ii

struct _ll ll_fun (void) { struct _ll ll = {1L,2L}; return ll;}
// CHECK-32: define void @ll_fun(%struct._ll* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define i128 @ll_fun()
// CHECK-64: ret i128

struct _LL LL_fun (void) { struct _LL LL = {1LL,2LL}; return LL;}
// CHECK-32: define void @LL_fun(%struct._LL* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define i128 @LL_fun()
// CHECK-64: ret i128

struct _ff ff_fun (void) { struct _ff ff = {1.0F, 2.0F}; return ff;}
// CHECK-32: define void @ff_fun(%struct._ff* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define %struct._ff @ff_fun()
// CHECK-64: ret %struct._ff

struct _dd dd_fun (void) { struct _dd dd = {1.0,-1.0}; return dd;}
// CHECK-32: define void @dd_fun(%struct._dd* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define i128 @dd_fun()
// CHECK-64: ret i128

struct _DD DD_fun (void) { struct _DD DD = {1.0L, 0.0L}; return DD;}
// CHECK-32: define void @DD_fun(%struct._DD* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define %struct._DD @DD_fun()
// CHECK-64: ret %struct._DD

struct _vv vv_fun (void) { struct _vv vv = {{0}, {1}}; return vv;}
// CHECK-32: define void @vv_fun(%struct._vv* inreg noalias sret %agg.result)
// CHECK-32: ret void
// CHECK-64: define %struct._vv @vv_fun()
// CHECK-64: ret %struct._vv

// Check Darwin64 specials...

#if __LP64__
// These are all 16bytes, and follow a "gotcha" rule that 16bytes
// trumps the specified rule set.
struct _c16 c16_fun () { struct _c16 c16 = {0}; return c16;}
// CHECK-64: define i128 @c16_fun()
// CHECK-64: ret i128
struct _s16 s16_fun () { struct _s16 s16 = {0}; return s16;}
// CHECK-64: define i128 @s16_fun()
// CHECK-64: ret i128
struct _i16 i16_fun () { struct _i16 i16 = {0}; return i16;}
// CHECK-64: define i128 @i16_fun()
// CHECK-64: ret i128
struct _L16 L16_fun () { struct _L16 L16 = {0}; return L16;}
// CHECK-64: define i128 @L16_fun()
// CHECK-64: ret i128
struct _f16 f16_fun () { struct _f16 f16 = {0}; return f16;}
// CHECK-64: define i128 @f16_fun()
// CHECK-64: ret i128
struct _d16 d16_fun () { struct _d16 d16 = {0}; return d16;}
// CHECK-64: define i128 @d16_fun()
// CHECK-64: ret i128
struct _D16 D16_fun () { struct _D16 D16 = {0}; return D16;}
// CHECK-64: define i128 @D16_fun()
// CHECK-64: ret i128
struct _v16 v16_fun () { struct _v16 v16 = {{0}}; return v16;}
// CHECK-64: define i128 @v16_fun()
// CHECK-64: ret i128
struct _m16 m16_fun () { struct _m16 m16 = {0}; return m16;}
// CHECK-64: define i128 @m16_fun()
// CHECK-64: ret i128

// Embedded unions are treated as integer blobs, it seems.
struct _e_u eu_fun () { struct _e_u eu = {1,2.0,{10.5L}}; return eu;}
// CHECK-64: define %struct._e_u @eu_fun()
// CHECK-64: ret %struct._e_u

// But .. we walk the embedded float structs filling up float regs.
struct _n1 n1_fun () { struct _n1 n1 = {1.0, {2.0}, {3.0}, 5}; return n1;}
// CHECK-64: define %struct._n1 @n1_fun()
// CHECK-64: ret %struct._n1

// and when there's an embedded 0-sized, it is ignored.
struct _n2 n2_fun () { struct _n2 n2 = {1.0, {2.0}, {}, 5}; return n2;}
// CHECK-64: define %struct._n2 @n2_fun()
// CHECK-64: ret %struct._n2

// GCC returns the non-flexible part of a dynamically allocated struct
// as if the trailing member didn't exist (modulo padding to align the
// trailing array appropriately).....
struct _fa fa_fun () { struct _fa fa = {sizeof(struct _fa), 0.1, 0.2 }; return fa;}
// CHECK-64: define %struct._fa @fa_fun()
// CHECK-64: ret %struct._fa

// ... likewise ... for static allocation.
// This seems like another bug, essentially, it's impossible to return such
// a struct...
// ... but is it ABI?
struct _fa fa2_fun () {
  static struct _fa fa = {
    sizeof(struct _fa),
    0.1, 0.2,
    { 3.0, 4.0, 5.0, 6.0, 7.0 }
  };
  return fa;
}
// CHECK-64: define %struct._fa @fa2_fun()
// CHECK-64: ret %struct._fa

struct _pad_test pad_test_fun () { 
  struct _pad_test pt = { 5.0 }; return pt;}
// CHECK-64: define %struct._pad_test @pad_test_fun()
// CHECK-64: ret %struct._pad_test

struct _pad_test_2 pad_test2_fun () { struct _pad_test_2 p2 = {1}; return p2;}
// CHECK-64: define void @pad_test2_fun(%struct._pad_test_2* inreg noalias sret %agg.result)
// CHECK-64: ret void

// This is a case to test one limit of the regs return rule...
struct _special_max max_fun (void) {
struct _special_max SM = {
  0,1,2,3,4,5,6,7,8.0F,9,10,11,12,13,14,15,
  'a','b','c','d','e','f','g','h',
  100, 200, 300,
  1000,2000,
  1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}};
 return SM;
}
// CHECK-64: define %struct._special_max @max_fun()
// CHECK-64: ret %struct._special_max

// ... a couple of cases that should return by sret on D64,
struct _too_big_1 too_big_one (void) {
struct _too_big_1 TB1 = {
  0,1,2,3,4,5,6,7,
  'a',
  1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}};
 return TB1;
}
// CHECK-64: define void @too_big_one(%struct._too_big_1* inreg noalias sret %agg.result)
// CHECK-64: ret void

struct _too_big_2 too_big_two (void) {
struct _too_big_2 TB2 = {
  0,1,2,3,4,5,6,7,
  1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0, 14.0,
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}};
 return TB2;
}
// CHECK-64: define void @too_big_two(%struct._too_big_2* inreg noalias sret %agg.result)
// CHECK-64: ret void

struct _too_big_3 too_big_three (void) {
struct _too_big_3 TB3 = {
  0,1,2,3,4,5,6,7,
  1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}};
 return TB3;
}
// CHECK-64: define void @too_big_three(%struct._too_big_3* inreg noalias sret %agg.result)
// CHECK-64: ret void

struct _bad_vect_type bad_vect_type (void) {
struct _bad_vect_type BV1 = {
  0,1,2,3,4,5,6,7,
  1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,
  {0,1}
 };
 return BV1;
}
// CHECK-64: define void @bad_vect_type(%struct._bad_vect_type* inreg noalias sret %agg.result)
// CHECK-64: ret void

#endif
