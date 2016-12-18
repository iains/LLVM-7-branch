#include "llvm/Support/string_compat.h"
#include "gtest/gtest.h"

#if !HAVE_STRNLEN
using namespace llvm;
namespace {

class StrnlenTest : public ::testing::Test {};

TEST_F(StrnlenTest, Basics) {
  EXPECT_EQ(0u, strnlen("", 0));
  EXPECT_EQ(0u, strnlen("", 1));
  EXPECT_EQ(0u, strnlen("a", 0));
  EXPECT_EQ(1u, strnlen("a", 1));
  EXPECT_EQ(1u, strnlen("a", 2));
  EXPECT_EQ(1u, strnlen("aa", 1));
  EXPECT_EQ(2u, strnlen("aa", 2));
  EXPECT_EQ(2u, strnlen("aabbcc", 2));
  EXPECT_EQ(0u, strnlen("aabbcc", 0));
}
}
#endif
