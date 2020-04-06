#ifndef MINES_SRC_UTIL_EXPECT_HH_1586204755599706988_
#define MINES_SRC_UTIL_EXPECT_HH_1586204755599706988_

#include <cassert>

#if defined(__clang__)
#  define ASSUME(cond) __builtin_assume(cond)
#elif defined(__GNUC__)
#  define ASSUME(cond)                                                         \
    if (!(cond))                                                               \
    __builtin_unreachable()
#else
#  define ASSUME(cond) (void)0
#endif

#ifdef NDEBUG
#  define EXPECT(cond) ASSUME(cond)
#else
#  define EXPECT(cond) assert(cond)
#endif

#endif // MINES_SRC_UTIL_EXPECT_HH_1586204755599706988_
