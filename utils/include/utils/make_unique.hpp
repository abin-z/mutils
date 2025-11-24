// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Abin

/**
 * @file make_unique.hpp
 * @brief 模仿 C++14 的 std::make_unique 实现
 * @author abin
 * @date 2025-11-25
 */

#ifndef __GUARD_MAKE_UNIQUE_HPP_INCLUDE_GUARD__
#define __GUARD_MAKE_UNIQUE_HPP_INCLUDE_GUARD__

#include <memory>

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

#endif  // __GUARD_MAKE_UNIQUE_HPP_INCLUDE_GUARD__