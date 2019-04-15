// RUN: %clang_cc1  -triple powerpc-apple-macosx10.5 -emit-llvm %s -malign-power -disable-llvm-optzns -fno-common -o - | \
// RUN: FileCheck %s  --check-prefix=CHECK-C --check-prefix=CHECK-P
// RUN: %clang_cc1  -triple powerpc-apple-macosx10.5 -emit-llvm %s -malign-natural -disable-llvm-optzns -fno-common -o - | \
// RUN: FileCheck %s  --check-prefix=CHECK-C --check-prefix=CHECK-N

#define TT  long long
#define TT_EL long long

#include "abi-inspector.h"

// CHECK-C: %struct._Ts = type { i64 }
// CHECK-C: %union._Tu = type { i64 }
// CHECK-C: %struct._Tcs = type { i64, i8, [7 x i8] }
// CHECK-C: %struct._Tzs = type { i64, [2 x %struct.anon] }
// CHECK-C: %struct.anon = type {}
// CHECK-C: %struct._TAcs = type { i64, i8, [7 x i8] }
// CHECK-P: %struct._cTs = type { i8, i64 }
// CHECK-N: %struct._cTs = type { i8, [7 x i8], i64 }
// CHECK-P: %struct._cSTs = type { i8, %struct._Ts }
// CHECK-N: %struct._cSTs = type { i8, [7 x i8], %struct._Ts }
// CHECK-C: %struct._sta5 = type { [5 x i64] }
// CHECK-C: %struct._stp = type { i64 }
// CHECK-C: %struct._sctp = type <{ i8, i64 }>
// CHECK-C: %union._utp = type { i64 }

// CHECK-C: @T_size = global i32 8
// CHECK-C: @T_align = global i32 8
// CHECK-C: @gT_size = global i32 8
// CHECK-C: @gT_align = global i32 8

// CHECK-C: @gA_size = global i32 40
// CHECK-C: @gA_align = global i32 8
// CHECK-C: @gA = global [5 x i64] zeroinitializer, align 8
// CHECK-C: @a0 = global i64* getelementptr inbounds ([5 x i64], [5 x i64]* @gA, i32 0, i32 0), align 4
// CHECK-C: @a2 = global i64* bitcast (i8* getelementptr (i8, i8* bitcast ([5 x i64]* @gA to i8*), i64 16) to i64*), align 4

// CHECK-C: @ST_t_size = global i32 8
// CHECK-C: @ST_t_align = global i32 8
// CHECK-C: @g_ST_t_size = global i32 8
// CHECK-C: @g_ST_t_align = global i32 8
// CHECK-C: @g_ST_t_align_t = global i32 4
// CHECK-C: @ST_t_offs_b = global i32 0
// CHECK-C: @UT_t_size = global i32 8
// CHECK-C: @UT_t_align = global i32 8
// CHECK-C: @g_UT_t_size = global i32 8
// CHECK-C: @g_UT_t_align = global i32 8
// CHECK-C: @g_UT_t_align_t = global i32 4
// CHECK-C: @UT_t_offs_t = global i32 0
// CHECK-C: @Sbc_t_size = global i32 16
// CHECK-C: @Sbc_t_align = global i32 8
// CHECK-C: @g_STc_t_size = global i32 16
// CHECK-C: @g_STc_t_align = global i32 8
// CHECK-C: @g_STc_t_align_t = global i32 4
// CHECK-C: @g_STc_t_align_c = global i32 1
// CHECK-C: @STc_t_off_t = global i32 0
// CHECK-C: @STc_t_off_c = global i32 8
// CHECK-C: @STz_t_size = global i32 8
// CHECK-C: @STz_t_align = global i32 8
// CHECK-C: @g_STz_t_size = global i32 8
// CHECK-C: @g_STz_t_align = global i32 8
// CHECK-C: @g_STz_t_align_t = global i32 4
// CHECK-C: @g_STz_t_align_z = global i32 1
// CHECK-C: @STz_t_off_t = global i32 0
// CHECK-C: @STz_t_off_z = global i32 8
// CHECK-C: @STAc_t_size = global i32 16
// CHECK-C: @STAc_t_align = global i32 16
// CHECK-C: @g_STAc_t_size = global i32 16
// CHECK-C: @g_STAc_t_align = global i32 16
// CHECK-C: @g_STAc_t_align_t = global i32 16
// CHECK-C: @g_STAc_t_align_c = global i32 1
// CHECK-C: @STAc_t_off_t = global i32 0
// CHECK-C: @STAc_t_off_c = global i32 8
// CHECK-P: @ScT_t_size = global i32 12
// CHECK-P: @ScT_t_align = global i32 4
// CHECK-N: @ScT_t_size = global i32 16
// CHECK-N: @ScT_t_align = global i32 8
// CHECK-P: @g_ScT_t_size = global i32 12
// CHECK-P: @g_ScT_t_align = global i32 4
// CHECK-N: @g_ScT_t_size = global i32 16
// CHECK-N: @g_ScT_t_align = global i32 8
// CHECK-C: @g_ScT_t_align_t = global i32 4
// CHECK-C: @g_ScT_t_align_c = global i32 1
// CHECK-P: @ScT_t_off_b = global i32 4
// CHECK-N: @ScT_t_off_b = global i32 8
// CHECK-C: @ScT_t_off_c = global i32 0
// CHECK-P: @ScST_t_size = global i32 12
// CHECK-P: @ScST_t_align = global i32 4
// CHECK-N: @ScST_t_size = global i32 16
// CHECK-N: @ScST_t_align = global i32 8
// CHECK-P: @g_ScST_t_size = global i32 12
// CHECK-P: @g_ScST_t_align = global i32 4
// CHECK-N: @g_ScST_t_size = global i32 16
// CHECK-N: @g_ScST_t_align = global i32 8
// CHECK-P: @g_ScST_t_align_t = global i32 4
// CHECK-N: @g_ScST_t_align_t = global i32 8
// CHECK-C: @g_ScST_t_align_c = global i32 1
// CHECK-P: @ScST_t_off_t = global i32 4
// CHECK-N: @ScST_t_off_t = global i32 8
// CHECK-C: @ScST_t_off_c = global i32 0
// CHECK-C: @STA5_t_size = global i32 40
// CHECK-C: @STA5_t_align = global i32 8
// CHECK-C: @g_STA5_t_size = global i32 40
// CHECK-C: @g_STA5_t_align = global i32 8
// CHECK-C: @g_STA5_t_align_t = global i32 4
// CHECK-C: @STA5_t_off_t = global i32 0
// CHECK-C: @STp_t_size = global i32 8

// CHECK-P: @STp_t_align = global i32 8
// CHECK-N: @STp_t_align = global i32 1
// CHECK-C: @g_STp_t_size = global i32 8
// CHECK-P: @g_STp_t_align = global i32 8
// CHECK-N: @g_STp_t_align = global i32 1
// CHECK-C: @g_STp_t_align_t = global i32 1

// CHECK-C: @STp_t_off_t = global i32 0
// CHECK-C: @ScTp_t_size = global i32 9
// CHECK-C: @ScTp_t_align = global i32 1
// CHECK-C: @g_ScTp_t_size = global i32 9
// CHECK-C: @g_ScTp_t_align = global i32 1
// CHECK-C: @g_ScTp_t_align_t = global i32 1
// CHECK-C: @ScTp_t_off_t = global i32 1
// CHECK-C: @UTp_t_size = global i32 8

// CHECK-P: @UTp_t_align = global i32 8
// CHECK-N: @UTp_t_align = global i32 1
// CHECK-C: @g_UTp_t_size = global i32 8
// CHECK-P: @g_UTp_t_align = global i32 8
// CHECK-N: @g_UTp_t_align = global i32 1

// CHECK-C: @g_UTp_t_align_t = global i32 1
// CHECK-C: @UTp_t_off_t = global i32 0
// CHECK-C: @gT_extern = global i64 0, align 8
// CHECK-C: @g_ST_t = global %struct._Ts zeroinitializer, align 8
// CHECK-C: @g_UT_t = global %union._Tu zeroinitializer, align 8
// CHECK-C: @gA_extern = external global [5 x i64], align 8
// CHECK-C: @gT = global i64 0, align 8
// CHECK-C: @g_STc_t = global %struct._Tcs zeroinitializer, align 8
// CHECK-C: @g_STz_t = global %struct._Tzs zeroinitializer, align 8
// CHECK-C: @g_STAc_t = global %struct._TAcs zeroinitializer, align 16
// CHECK-P: @g_ScT_t = global %struct._cTs zeroinitializer, align 4
// CHECK-N: @g_ScT_t = global %struct._cTs zeroinitializer, align 8
// CHECK-P: @g_ScST_t = global %struct._cSTs zeroinitializer, align 4
// CHECK-N: @g_ScST_t = global %struct._cSTs zeroinitializer, align 8
// CHECK-C: @g_STA5_t = global %struct._sta5 zeroinitializer, align 8

// hmmm...
// CHECK-P: @g_STp_t = global %struct._stp zeroinitializer, align 8
// CHECK-N: @g_STp_t = global %struct._stp zeroinitializer, align 1
// CHECK-C: @g_ScTp_t = global %struct._sctp zeroinitializer, align 1
// CHECK-P: @g_UTp_t = global %union._utp zeroinitializer, align 8
// CHECK-N: @g_UTp_t = global %union._utp zeroinitializer, align 1
