#ifndef LLVM_SUPPORT_STRING_COMPAT_H
#define LLVM_SUPPORT_STRING_COMPAT_H

#include <stdlib.h>

#include "llvm/Config/llvm-config.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !HAVE_STRNLEN
size_t strnlen(const char *s, size_t maxlen);
#endif /* LLVM_HAS_STRNLEN */

#ifdef __cplusplus
}
#endif

#endif /* LLVM_SUPPORT_STRING_COMPAT_H */
