/*
 * 简介: 校验工具集
 */

#pragma once
#include <cstdint>
#include <string>

namespace checkutils
{

// ---------------- CRC ----------------
uint8_t crc8(const std::string& data);
uint8_t crc8_file(const std::string& filepath);

uint16_t crc16(const std::string& data);
uint16_t crc16_file(const std::string& filepath);

uint32_t crc32(const std::string& data);
uint32_t crc32_file(const std::string& filepath);

// ---------------- 和校验 ----------------
uint8_t sum8(const std::string& data);  // 1字节和校验
uint8_t sum8_file(const std::string& filepath);

uint16_t sum16(const std::string& data);  // 2字节和校验
uint16_t sum16_file(const std::string& filepath);

// ---------------- 异或校验 ----------------
uint8_t xor8(const std::string& data);
uint8_t xor8_file(const std::string& filepath);

// ---------------- LRC（纵向冗余校验） ----------------
uint8_t lrc8(const std::string& data);
uint8_t lrc8_file(const std::string& filepath);

// ---------------- Fletcher 校验 ----------------
uint16_t fletcher16(const std::string& data);
uint16_t fletcher16_file(const std::string& filepath);

uint32_t fletcher32(const std::string& data);
uint32_t fletcher32_file(const std::string& filepath);

}  // namespace checkutils
