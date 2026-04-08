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

#include <cstdint>

namespace timeutils
{

// 注: DateTime相关功能推荐使用date库: https://github.com/HowardHinnant/date

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
