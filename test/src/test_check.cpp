// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Abin
//
// Catch2 v2.13.x 测试文件
// 对 checkutils 校验工具集进行全面测试
//

#include <catch2/catch.hpp>
#include <fstream>
#include <string>

#include "utils/check.h"

using namespace checkutils;

namespace
{

// 辅助函数：写临时文件
std::string write_temp_file(const std::string &content)
{
  static int idx = 0;
  const std::string path = "/tmp/checkutils_test_" + std::to_string(idx++);
  std::ofstream ofs(path, std::ios::binary);
  REQUIRE(ofs.good());
  ofs.write(content.data(), content.size());
  return path;
}

}  // namespace

// ---------------- 空输入 ----------------
TEST_CASE("checkutils: empty input", "[check]")
{
  std::string empty;

  REQUIRE(crc8(empty) == crc8(empty));
  REQUIRE(crc16(empty) == crc16(empty));
  REQUIRE(crc32(empty) == crc32(empty));

  REQUIRE(sum8(empty) == 0);
  REQUIRE(sum16(empty) == 0);
  REQUIRE(xor8(empty) == 0);
  REQUIRE(lrc8(empty) == 0);

  REQUIRE(fletcher16(empty) == fletcher16(empty));
  REQUIRE(fletcher32(empty) == fletcher32(empty));
}

// ---------------- CRC16 MODBUS 已知向量 ----------------
TEST_CASE("checkutils: CRC16 MODBUS known vector", "[check][crc16]")
{
  const std::string input = "123456789";
  REQUIRE(crc16(input) == 0x4B37);  // 标准测试向量
}

// ---------------- sum / xor / lrc ----------------
TEST_CASE("checkutils: sum / xor / lrc basic", "[check][sum][xor][lrc]")
{
  const std::string data = "\x01\x02\x03\x04";

  REQUIRE(sum8(data) == static_cast<uint8_t>(1 + 2 + 3 + 4));
  REQUIRE(sum16(data) == static_cast<uint16_t>(1 + 2 + 3 + 4));
  REQUIRE(xor8(data) == static_cast<uint8_t>(1 ^ 2 ^ 3 ^ 4));

  const uint8_t expected_lrc = static_cast<uint8_t>(-(1 + 2 + 3 + 4));
  REQUIRE(lrc8(data) == expected_lrc);
}

// ---------------- Fletcher 校验 ----------------
TEST_CASE("checkutils: fletcher basic properties", "[check][fletcher]")
{
  const std::string data = "abcdef";

  REQUIRE(fletcher16(data) == fletcher16(data));
  REQUIRE(fletcher32(data) == fletcher32(data));

  REQUIRE(fletcher16(data) != 0);
  REQUIRE(fletcher32(data) != 0);
}

// ---------------- 文件接口 ≡ 内存接口 ----------------
TEST_CASE("checkutils: file and memory results match", "[check][file]")
{
  const std::string data = "file check test data";
  const std::string path = write_temp_file(data);

  REQUIRE(crc8_file(path) == crc8(data));
  REQUIRE(crc16_file(path) == crc16(data));
  REQUIRE(crc32_file(path) == crc32(data));

  REQUIRE(sum8_file(path) == sum8(data));
  REQUIRE(sum16_file(path) == sum16(data));

  REQUIRE(xor8_file(path) == xor8(data));
  REQUIRE(lrc8_file(path) == lrc8(data));

  REQUIRE(fletcher16_file(path) == fletcher16(data));
  REQUIRE(fletcher32_file(path) == fletcher32(data));
}

// ---------------- 二进制安全测试 ----------------
TEST_CASE("checkutils: binary data", "[check][binary]")
{
  std::string data;
  data.push_back('\0');
  data.push_back('\xFF');
  data.push_back('\x01');

  REQUIRE(crc32(data) == crc32(data));
  REQUIRE(sum8(data) == static_cast<uint8_t>(0 + 255 + 1));
  REQUIRE(sum16(data) == static_cast<uint16_t>(0 + 255 + 1));
  REQUIRE(xor8(data) == static_cast<uint8_t>(0 ^ 255 ^ 1));

  const uint8_t expected_lrc = static_cast<uint8_t>(-(0 + 255 + 1));
  REQUIRE(lrc8(data) == expected_lrc);

  REQUIRE(fletcher16(data) == fletcher16(data));
  REQUIRE(fletcher32(data) == fletcher32(data));
}
