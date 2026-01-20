// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Abin

/**
 * @file string_utils.h
 * @brief string工具类
 * @author abin
 * @date 2025-12-30
 */

#ifndef __GUARD_STRING_UTILS_H_INCLUDE_GUARD__
#define __GUARD_STRING_UTILS_H_INCLUDE_GUARD__

#include <string>
#include <vector>

namespace stringutils
{
std::vector<std::string> split(const std::string &str, char delimiter);
std::string join(const std::vector<std::string> &parts, const std::string &delimiter);
std::string to_lower(const std::string &str);
std::string to_upper(const std::string &str);
std::string trim(const std::string &str);
std::string trim_left(const std::string &str);
std::string trim_right(const std::string &str);

bool starts_with(const std::string &str, const std::string &prefix);
bool ends_with(const std::string &str, const std::string &suffix);
bool contains(const std::string &str, const std::string &substr);

}  // namespace stringutils

#endif  // __GUARD_STRING_UTILS_H_INCLUDE_GUARD__