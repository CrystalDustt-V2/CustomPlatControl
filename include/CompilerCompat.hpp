#pragma once

// [Geode v5.0.0-beta.3 Migration]
// Work around MSVC not exposing GCC-style __builtin_* names used by
// Geode's nontype_functional dependency.
#if defined(_MSC_VER) && !defined(__clang__)
    #include <cstring>

    #ifndef __builtin_memset
        #define __builtin_memset std::memset
    #endif

    #ifndef __builtin_memcmp
        #define __builtin_memcmp std::memcmp
    #endif

    #ifndef __builtin_unreachable
        #define __builtin_unreachable() __assume(0)
    #endif
#endif
