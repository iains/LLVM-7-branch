#include <stdbool.h>
#include <stddef.h>

#ifndef TT
#error "define the type/element type you want to test and then include this"
#error "TT is the type and TT_EL is the aggregate element type"
#endif

int T_size = sizeof(TT_EL);
int T_align = __alignof__(TT_EL);

TT_EL gT;
TT_EL gT_extern;

unsigned gT_size = sizeof(gT);
unsigned gT_align = __alignof__(gT);

TT_EL gA[5];
extern TT_EL gA_extern[5];

unsigned gA_size = sizeof(gA);
unsigned gA_align = __alignof__(gA);

TT_EL *a0 = &gA[0];
TT_EL *a2 = &gA[2];

// =====

typedef struct _Ts {
 TT_EL t;
} ST_t;

ST_t g_ST_t;

unsigned ST_t_size = sizeof(ST_t);
unsigned ST_t_align = __alignof__(ST_t);

unsigned g_ST_t_size = sizeof(g_ST_t);
unsigned g_ST_t_align = __alignof__(g_ST_t);
unsigned g_ST_t_align_t = __alignof__(g_ST_t.t);
unsigned ST_t_offs_b = offsetof(struct _Ts, t);

// =====

typedef union _Tu {
 TT_EL t;
} UT_t;

UT_t g_UT_t;

unsigned UT_t_size = sizeof(UT_t);
unsigned UT_t_align = __alignof__(UT_t);

unsigned g_UT_t_size = sizeof(g_UT_t);
unsigned g_UT_t_align = __alignof__(g_UT_t);
unsigned g_UT_t_align_t = __alignof__(g_UT_t.t);
unsigned UT_t_offs_t = offsetof(struct _Ts, t);

//=== 

typedef struct _Tcs {
 TT_EL t;
 char c;
} STc_t;

STc_t g_STc_t;

unsigned Sbc_t_size = sizeof(STc_t);
unsigned Sbc_t_align = __alignof__(STc_t);

unsigned g_STc_t_size = sizeof(g_STc_t);
unsigned g_STc_t_align = __alignof__(g_STc_t);
unsigned g_STc_t_align_t = __alignof__(g_STc_t.t);
unsigned g_STc_t_align_c = __alignof__(g_STc_t.c);
unsigned STc_t_off_t = offsetof(struct _Tcs, t);
unsigned STc_t_off_c = offsetof(struct _Tcs, c);


//=== now padded with a zero-sized entity.

typedef struct _Tzs {
  TT_EL t;
  struct {
  } z[2];
} STz_t;

STz_t g_STz_t;

unsigned STz_t_size = sizeof(STz_t);
unsigned STz_t_align = __alignof__(STc_t);

unsigned g_STz_t_size = sizeof(g_STz_t);
unsigned g_STz_t_align = __alignof__(g_STz_t);
unsigned g_STz_t_align_t = __alignof__(g_STz_t.t);
unsigned g_STz_t_align_z = __alignof__(g_STz_t.z);
unsigned STz_t_off_t = offsetof(struct _Tzs, t);
unsigned STz_t_off_z = offsetof(struct _Tzs, z);

//=== this reports the excess alignment

typedef struct _TAcs {
 TT_EL __attribute__((__aligned__(16))) t ;
 char c;
} STAc_t;

STAc_t g_STAc_t;

unsigned STAc_t_size = sizeof(STAc_t);
unsigned STAc_t_align = __alignof__(STAc_t);

unsigned g_STAc_t_size = sizeof(g_STAc_t);
unsigned g_STAc_t_align = __alignof__(g_STAc_t);
unsigned g_STAc_t_align_t = __alignof__(g_STAc_t.t);
unsigned g_STAc_t_align_c = __alignof__(g_STAc_t.c);
unsigned STAc_t_off_t = offsetof(struct _TAcs, t);
unsigned STAc_t_off_c = offsetof(struct _TAcs, c);

//===

typedef struct _cTs {
 char c;
 TT_EL b;
} ScT_t;

ScT_t g_ScT_t;

unsigned ScT_t_size = sizeof(ScT_t);
unsigned ScT_t_align = __alignof__(ScT_t);

unsigned g_ScT_t_size = sizeof(g_ScT_t);
unsigned g_ScT_t_align = __alignof__(g_ScT_t);
unsigned g_ScT_t_align_t = __alignof__(g_ScT_t.b);
unsigned g_ScT_t_align_c = __alignof__(g_ScT_t.c);
unsigned ScT_t_off_b = offsetof(struct _cTs, b);
unsigned ScT_t_off_c = offsetof(struct _cTs, c);

//===

typedef struct _cSTs {
 char c;
 ST_t t;
} ScST_t;

ScST_t g_ScST_t;

unsigned ScST_t_size = sizeof(ScST_t);
unsigned ScST_t_align = __alignof__(ScST_t);

unsigned g_ScST_t_size = sizeof(g_ScST_t);
unsigned g_ScST_t_align = __alignof__(g_ScST_t);
unsigned g_ScST_t_align_t = __alignof__(g_ScST_t.t);
unsigned g_ScST_t_align_c = __alignof__(g_ScST_t.c);
unsigned ScST_t_off_t = offsetof(struct _cSTs, t);
unsigned ScST_t_off_c = offsetof(struct _cSTs, c);

//===

typedef struct _sta5 {
 TT_EL t[5];
} STA5_t;

STA5_t g_STA5_t;

unsigned STA5_t_size = sizeof(STA5_t);
unsigned STA5_t_align = __alignof__(STA5_t);

unsigned g_STA5_t_size = sizeof(g_STA5_t);
unsigned g_STA5_t_align = __alignof__(g_STA5_t);
unsigned g_STA5_t_align_t = __alignof__(g_STA5_t.t);
unsigned STA5_t_off_t = offsetof(struct _sta5, t);

//===

typedef struct _stp {
 TT_EL t __attribute__((__packed__));
} STp_t;

STp_t g_STp_t;

unsigned STp_t_size = sizeof(STp_t);
unsigned STp_t_align = __alignof__(STp_t);

unsigned g_STp_t_size = sizeof(g_STp_t);
unsigned g_STp_t_align = __alignof__(g_STp_t);
unsigned g_STp_t_align_t = __alignof__(g_STp_t.t);
unsigned STp_t_off_t = offsetof(struct _stp, t);

//===

typedef struct _sctp {
  char c;
  TT_EL t __attribute__((__packed__));
} ScTp_t;

ScTp_t g_ScTp_t;

unsigned ScTp_t_size = sizeof(ScTp_t);
unsigned ScTp_t_align = __alignof__(ScTp_t);

unsigned g_ScTp_t_size = sizeof(g_ScTp_t);
unsigned g_ScTp_t_align = __alignof__(g_ScTp_t);
unsigned g_ScTp_t_align_t = __alignof__(g_ScTp_t.t);
unsigned ScTp_t_off_t = offsetof(struct _sctp, t);

//===

typedef union _utp {
 TT_EL t __attribute__((__packed__));
} UTp_t;

UTp_t g_UTp_t;

unsigned UTp_t_size = sizeof(UTp_t);
unsigned UTp_t_align = __alignof__(UTp_t);

unsigned g_UTp_t_size = sizeof(g_UTp_t);
unsigned g_UTp_t_align = __alignof__(g_UTp_t);
unsigned g_UTp_t_align_t = __alignof__(g_UTp_t.t);
unsigned UTp_t_off_t = offsetof(union _utp, t);

//=== 

TT ret_glob (void)
{
  return gT_extern;
}

ST_t ret_glob_str (void)
{
  return g_ST_t;
}

UT_t ret_glob_unn (void)
{
  return g_UT_t;
}

TT pass_tt (TT a, TT b)
{
  return b;
}

ST_t pass_ts (ST_t a, ST_t b)
{
  return b;
}

UT_t pass_tu (UT_t a, UT_t b)
{
  return b;
}

extern TT f (TT, TT *, TT, TT *);

TT *call_arr(void)
{
  TT_EL x;
  TT_EL *t = &gA_extern[2];
  x = f (gT, &gA_extern[1], gA_extern[2], t);
  return t;
}

extern void abort(void) __attribute__((__noreturn__));

TT foo (TT a, TT *b, TT c, TT *d)
{
  if (b == &gT_extern)
    abort();

  if (b != &gA[1])
    abort();

  if (d != &gA[2])
    abort();

  return a;
}
