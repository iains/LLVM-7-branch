// TODO: bool vectors.
typedef int __attribute__((mode(QI))) qi;
typedef qi  __attribute__((vector_size (16))) v16qi;

typedef int __attribute__((mode(HI))) hi;
typedef hi  __attribute__((__vector_size__(16))) v8hi
;
typedef int __attribute__((mode(SI))) si;
typedef si  __attribute__((__vector_size__(16))) v4si;

typedef float __attribute__((mode(SF))) sf;
typedef sf __attribute__((__vector_size__(16))) v4sf;

/* PowerPC Darwin has a special case where it can use two
   VSX types without any VSX hardware (implemented by
   expansions, and not really that useful).  These are
   returned as an i128 blob.
*/

typedef int __attribute__((mode(DI))) di;
typedef di  __attribute__((__vector_size__(16))) v2di;

typedef float __attribute__((mode(DF))) df;
typedef df __attribute__((__vector_size__(16))) v2df;
