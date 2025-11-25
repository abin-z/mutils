// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Abin

/**
 * @file make_unique.hpp
 * @brief C++11 fallback for std::make_unique (C++14+ uses std::make_unique)
 * @author abin
 * @date 2025-11-25
 */

#ifndef __GUARD_MAKE_UNIQUE_HPP_INCLUDE_GUARD__
#define __GUARD_MAKE_UNIQUE_HPP_INCLUDE_GUARD__

#include <memory>

// ----------------------------------------------
// 版本检测（兼容 MSVC / GCC / Clang）
// ----------------------------------------------

// 判断是否有 C++14 的 make_unique 特性
// 标准检测：C++14 <memory> 会定义该宏
#if defined(__cpp_lib_make_unique) && (__cpp_lib_make_unique >= 201304)

#define UTILS_HAS_MAKE_UNIQUE 1

// MSVC 特判（即便 __cplusplus 不正确，也能判断 VS2015 Update 2+)
#elif defined(_MSC_VER)
// VS version table:
// 1900 = VS2015
// 1900 + Update 2 (really ~1910+) 才开始全面支持 C++14 make_unique
#if _MSC_VER >= 1900
#define UTILS_HAS_MAKE_UNIQUE 1
#else
#define UTILS_HAS_MAKE_UNIQUE 0
#endif

// GCC/Clang：使用正常的标准版本号判断
#elif __cplusplus >= 201402L
#define UTILS_HAS_MAKE_UNIQUE 1

#else
#define UTILS_HAS_MAKE_UNIQUE 0
#endif

// ----------------------------------------------
//   分支：C++14+ → 直接使用 std::make_unique
// ----------------------------------------------

#if UTILS_HAS_MAKE_UNIQUE

namespace utils
{
using std::make_unique;
}  // namespace utils

// ----------------------------------------------
//   分支：C++11 fallback
// ----------------------------------------------

#else  // fallback

namespace utils
{
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
  static_assert(!std::is_array<T>::value, "make_unique does not support arrays");
  static_assert(!std::is_reference<T>::value, "T must not be a reference");

  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}  // namespace utils

#endif  // UTILS_HAS_MAKE_UNIQUE

#endif  // __GUARD_MAKE_UNIQUE_HPP_INCLUDE_GUARD__
