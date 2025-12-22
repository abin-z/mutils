// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Abin

/**
 * @file url_codec.h
 * @brief URL编解码
 * @author abin
 * @date 2025-11-22
 */

#ifndef __GUARD_URL_CODEC_H_INCLUDE_GUARD__
#define __GUARD_URL_CODEC_H_INCLUDE_GUARD__

#include <string>

namespace codec
{

// ---------------- URL 编码/解码 ----------------
std::string url_encode(const std::string &value);
std::string url_decode(const std::string &value);

}  // namespace codec

#endif  // __GUARD_URL_CODEC_H_INCLUDE_GUARD__