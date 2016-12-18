#include "llvm/Support/string_compat.h"

#if !HAVE_STRNLEN
size_t strnlen(const char *s, size_t maxlen) {
  size_t rv = 0;
  while (maxlen-- > 0 && *s++)
    ++rv;
  return rv;
}
#endif
