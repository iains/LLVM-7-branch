#include <stdbool.h>

struct _e {};
struct _b { bool x; };
struct _c { char x; };
struct _s { short s; };
struct _i { int i; };
struct _l { long l; };
struct _L { long long ll; };
struct _f { float f; };
struct _d { double d; };
struct _D { long double d; };
struct _v { v16qi v; };

struct _bb { bool a, b; };
struct _cc { char a, b; };
struct _ss { short a, b; };
struct _ii { int a, b; };
struct _ll { long a, b; };
struct _LL { long long a, b; };
struct _ff { float a, b; };
struct _dd { double a, b; };
struct _DD { long double a, b; };
struct _vv { v16qi a, b; };

// Darwin64 has special rules for aggregates.
// There's a total of :
// 8 GPRs
// 13 FPRs
// 12 VRs.
// ... available to contain the aggregate's members...
// There are, however, special shadowing rules about the integer ones...
// imagine that they correspond to an image of the memory of the first
// 64bytes of the aggregate - regardless of the content.  So, for example,
// if the agg. begins with 8 long long values, those would all end up in
// GPRs .. conversely if the agg. begins with 8 double values, then all
// GPRs would be shadowed and unusable for int values.

// **BUT** looks like we have to work around some unintentional behaviour
// where every 16byte struct is returned directly (i.e. as int128)
// for Darwin64.

struct _c16 { char a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p; };
struct _s16 { short a,b,c,d,e,f,g,h; };
struct _i16 { int a,b,c,d; };
struct _L16 { long long a,b; };
struct _f16 { float a,b,c,d; };
struct _d16 { double a,b; };
struct _D16 { long double a; };
struct _v16 { v16qi a; };
struct _m16 { char a, b;  float f; char c; short x; int y;};

union _uex { long double D; char c; short s; int i; long l; long long L;
             float f; double d; };

// Embedded union
struct _e_u { int a; float b; union _uex u;};

// Nested structs (float reg use).
struct _n1 { float a; struct _d d; struct _D16 D; int i;};

// Nested structs (empty struct).
struct _n2 { float a; struct _d d; struct _e e; int i;};

// Flexible array member.
struct _fa { int x; float y, z; double fa[]; };

struct _special_max {
  int a,b,c,d,e,f,g;
  float ff;
  char a1,b1,c1,d1,e1,fx,g1,h1;
  char a2,b2,c2,d2,e2,fy,g2,h2;
  int p, q, r;
  short s,t;
  double f0,f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11;
  v16qi v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11;
};

// This works....
struct _pad_test {
  long long a,b,c,d,e,f,g,h;
  // Ignoring all the padding between float/vector
  float f0; double d0; v16qi v0;
  float f1;  v16qi v1; double d1; 
  float f2; double d2; v16qi v2;
  float f3;  v16qi v3; double d3;
  float f4; double d4; v16qi v4;
  float f5;  v16qi v5; double d5;
};

// You'd think this would work, because we have enough GPRs...
// ... but it seems that the process considers the offset of the
// int fields as part of the entity.
struct _pad_test_2 {
  float f0; double d0; v16qi v0;
  long long a,b,c,d,e;
  float f1;  v16qi v1; double d1;
  float f2; double d2; v16qi v2;
  float f3;  v16qi v3; double d3;
  float f4; double d4; v16qi v4;
  float f5;  v16qi v5; double d5;
};

// too many ints.. 
struct _too_big_1 {
  long a,b,c,d,e,f,g,h;
  char cc;
  double f0,f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12;
  v16qi v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11;
};

// too many floats...
struct _too_big_2 {
  long a,b,c,d,e,f,g,h;
  double f0,f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12, f13;
  v16qi v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11;
};

// too many vects...
struct _too_big_3 {
  long a,b,c,d,e,f,g,h;
  double f0,f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12;
  v16qi v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11, v12;
};

// too many vects...
struct _bad_vect_type {
  long a,b,c,d,e,f,g,h;
  double f0,f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12;
  v2di v0;
};
