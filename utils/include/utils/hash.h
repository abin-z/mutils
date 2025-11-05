/*
 * 简介: hash工具集, 依赖于OpenSSL
 */

#pragma once

#include <string>

namespace hashutils
{
// ------------------ MD5 ------------------
std::string md5(const std::string& data);
std::string md5_file(const std::string& filepath);

// ------------------ SHA1 ------------------
std::string sha1(const std::string& data);
std::string sha1_file(const std::string& filepath);

// ------------------ SHA224 ------------------
std::string sha224(const std::string& data);
std::string sha224_file(const std::string& filepath);

// ------------------ SHA256 ------------------
std::string sha256(const std::string& data);
std::string sha256_file(const std::string& filepath);

// ------------------ SHA384 ------------------
std::string sha384(const std::string& data);
std::string sha384_file(const std::string& filepath);

// ------------------ SHA512 ------------------
std::string sha512(const std::string& data);
std::string sha512_file(const std::string& filepath);
}  // namespace hashutils
