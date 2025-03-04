/**
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/Fmt.h"
#include "quill/TweakMe.h"
#include <sstream>
#include <string>

/**
 * Common type definitions etc
 */
#if !defined(QUILL_RDTSC_RESYNC_INTERVAL)
  #define QUILL_RDTSC_RESYNC_INTERVAL 700
#endif

#if !defined(QUILL_ACTIVE_LOG_LEVEL)
  #define QUILL_ACTIVE_LOG_LEVEL QUILL_LOG_LEVEL_TRACE_L3
#endif

#if !defined(QUILL_QUEUE_CAPACITY)
  #define QUILL_QUEUE_CAPACITY 131'072
#endif

/**
 * Convert number to string
 */
#define QUILL_AS_STR(x) #x
#define QUILL_STRINGIFY(x) QUILL_AS_STR(x)

/**
 * Likely
 */
#if defined(__GNUC__)
  #define QUILL_LIKELY(x) (__builtin_expect((x), 1))
  #define QUILL_UNLIKELY(x) (__builtin_expect((x), 0))
#else
  #define QUILL_LIKELY(x) (x)
  #define QUILL_UNLIKELY(x) (x)
#endif

/**
 * Require check
 */
#define QUILL_REQUIRE(expression, error)                                                           \
  do                                                                                               \
  {                                                                                                \
    if (QUILL_UNLIKELY(!(expression)))                                                             \
    {                                                                                              \
      printf("Quill fatal error: %s (%s:%d)\n", error, __FILE__, __LINE__);                        \
      std::abort();                                                                                \
    }                                                                                              \
  } while (0)

namespace quill
{
namespace detail
{
/**
 * Cache line size
 */
static constexpr size_t CACHELINE_SIZE{64u};
} // namespace detail

/**
 * Enum to select a timezone
 */
enum class Timezone : uint8_t
{
  LocalTime,
  GmtTime
};

} // namespace quill

#if !defined(QUILL_HAS_FILESYSTEM) && !defined(QUILL_HAS_EXPERIMENTAL_FILESYSTEM)
    #if defined(__cpp_lib_filesystem)
        #define QUILL_HAS_FILESYSTEM 1
    #elif defined(__cpp_lib_experimental_filesystem)
        #define QUILL_HAS_EXPERIMENTAL_FILESYSTEM 1
    #elif !defined(__has_include)
        #define QUILL_HAS_EXPERIMENTAL_FILESYSTEM 1
    #elif __has_include(<filesystem>)
        #define QUILL_HAS_FILESYSTEM 1
    #elif __has_include(<experimental/filesystem>)
        #define QUILL_HAS_EXPERIMENTAL_FILESYSTEM 1
    #endif

    // std::filesystem does not work on MinGW GCC 8: https://sourceforge.net/p/mingw-w64/bugs/737/
    #if defined(__MINGW32__) && defined(__GNUC__) && __GNUC__ == 8
        #undef QUILL_HAS_FILESYSTEM
        #undef QUILL_HAS_EXPERIMENTAL_FILESYSTEM
    #endif

    // no filesystem support before GCC 8: https://en.cppreference.com/w/cpp/compiler_support
    #if defined(__GNUC__) && !defined(__clang__) && __GNUC__ < 8
        #undef QUILL_HAS_FILESYSTEM
        // #undef QUILL_HAS_EXPERIMENTAL_FILESYSTEM
    #endif

    // no filesystem support before Clang 7: https://en.cppreference.com/w/cpp/compiler_support
    #if defined(__clang_major__) && __clang_major__ < 7
        #undef QUILL_HAS_FILESYSTEM
        #undef QUILL_HAS_EXPERIMENTAL_FILESYSTEM
    #endif

    // no filesystem support before MSVC 19.14: https://en.cppreference.com/w/cpp/compiler_support
    #if defined(_MSC_VER) && _MSC_VER < 1940
        #undef QUILL_HAS_FILESYSTEM
        #undef QUILL_HAS_EXPERIMENTAL_FILESYSTEM
    #endif

    // no filesystem support before iOS 13
    #if defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED < 130000
        #undef QUILL_HAS_FILESYSTEM
        #undef QUILL_HAS_EXPERIMENTAL_FILESYSTEM
    #endif

    // no filesystem support before macOS Catalina
    #if defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED < 101500
        #undef QUILL_HAS_FILESYSTEM
        #undef QUILL_HAS_EXPERIMENTAL_FILESYSTEM
    #endif
#endif

#ifndef QUILL_HAS_EXPERIMENTAL_FILESYSTEM
    #define QUILL_HAS_EXPERIMENTAL_FILESYSTEM 0
#endif

#ifndef QUILL_HAS_FILESYSTEM
    #define QUILL_HAS_FILESYSTEM 0
#endif

#if QUILL_HAS_EXPERIMENTAL_FILESYSTEM
#include <experimental/filesystem>
namespace quill::detail
{
namespace std_fs = std::experimental::filesystem;
} // namespace quill::detail
#elif QUILL_HAS_FILESYSTEM
#include <filesystem>
namespace quill::detail
{
namespace std_fs = std::filesystem;
} // namespace quill::detail
#endif

