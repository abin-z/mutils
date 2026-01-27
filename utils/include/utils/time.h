// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Abin

/**
 * @file time.h
 * @brief 时间相关工具函数（格式化时间字符串 / 时间戳）
 * 注: DateTime相关功能推荐使用date库: https://github.com/HowardHinnant/date
 *
 * 提供：
 *  - 获取当前时间字符串
 *  - 将指定时间点格式化为字符串
 *  - 获取当前时间戳（秒 / 毫秒）
 *
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

/**
 * @brief 获取当前时间的格式化字符串
 *
 * @param fmt_str 时间格式字符串，遵循 strftime 规则，额外支持 %f 毫秒（000–999，固定 3 位）
 *        支持常用格式说明符：
 *        %Y 年, %m 月, %d 日, %H 时, %M 分, %S 秒
 *        额外支持 %f 毫秒（000–999，固定 3 位）
 *        默认格式 "%Y-%m-%d %H:%M:%S"
 * @return 格式化后的时间字符串
 */
std::string now_time_string(const std::string &fmt_str = "%Y-%m-%d %H:%M:%S");

/**
 * @brief 将指定时间点格式化为字符串
 *
 * @param tp      时间点（system_clock::time_point）
 * @param fmt_str 时间格式字符串，规则同 now_time_string()
 * @return 格式化后的时间字符串
 */
std::string format_time_string(const std::chrono::system_clock::time_point &tp, const std::string &fmt_str);

/**
 * @brief 获取当前时间戳（秒）
 * @return 秒级时间戳
 */
int64_t current_timestamp_sec();

/**
 * @brief 获取当前时间戳（毫秒）
 * @return 毫秒级时间戳
 */
int64_t current_timestamp_millis();

}  // namespace timeutils

#endif  // __GUARD_TIME_H_INCLUDE_GUARD__
