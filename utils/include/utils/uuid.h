// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Abin

/**
 * @file uuid.h
 * @brief uuid 生成工具
 * @author abin
 * @date 2025-11-22
 */

#ifndef __GUARD_UUID_H_INCLUDE_GUARD__
#define __GUARD_UUID_H_INCLUDE_GUARD__

#include <string>

namespace uuid
{

// 生成随机 UUID v4
std::string uuidv4();

}  // namespace uuid

#endif  // __GUARD_UUID_H_INCLUDE_GUARD__