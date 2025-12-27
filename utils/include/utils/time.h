// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Abin

/**
 * @file time.h
 * @brief 时间相关的工具函数
 * @author abin
 * @date 2025-11-29
 */

#ifndef __GUARD_TIME_H_INCLUDE_GUARD__
#define __GUARD_TIME_H_INCLUDE_GUARD__

#include <chrono>
#include <cstdint>
#include <string>
namespace timeutils
{

// 获取当前时间字符串，可自定义格式
// fmt 格式示例: "%Y-%m-%d %H:%M:%S" 或 "%Y-%m-%d %H:%M:%S.%f"
std::string now_time_string(const std::string &fmt_str = "%Y-%m-%d %H:%M:%S");

std::string format_time_point(const std::chrono::system_clock::time_point &tp, const std::string &fmt_str);

// 获取当前时间的时间戳（秒级）
int64_t current_timestamp_sec();
// 获取当前时间的时间戳（毫秒级）
int64_t current_timestamp_millis();

}  // namespace timeutils

#endif  // __GUARD_TIME_H_INCLUDE_GUARD__