#include "utils/check.h"

#include <array>
#include <fstream>

namespace checkutils
{

// ---------------- 内部工具函数 ----------------
namespace
{
constexpr std::size_t BUFFER_SIZE = 8192;
using Buffer = std::array<char, BUFFER_SIZE>;

Buffer make_buffer()
{
    return {};
}

std::ifstream open_file(const std::string& filepath)
{
    return std::ifstream(filepath, std::ios::binary);
}
}  // namespace

// ---------------- CRC8 ----------------
static uint8_t crc8_update(uint8_t crc, const unsigned char* buf, size_t len)
{
    while (len--)
    {
        crc ^= *buf++;
        for (int i = 0; i < 8; ++i) crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1);
    }
    return crc;
}

uint8_t crc8(const std::string& data)
{
    return crc8_update(0x00, reinterpret_cast<const unsigned char*>(data.c_str()), data.size());
}

uint8_t crc8_file(const std::string& filepath)
{
    auto file = open_file(filepath);
    if (!file) return 0;

    uint8_t crc = 0x00;
    auto buffer = make_buffer();
    while (file.good())
    {
        file.read(buffer.data(), buffer.size());
        crc = crc8_update(crc, reinterpret_cast<const unsigned char*>(buffer.data()), file.gcount());
    }
    return crc;
}

// ---------------- CRC16 ----------------
static uint16_t crc16_update(uint16_t crc, const unsigned char* buf, size_t len)
{
    while (len--)
    {
        crc ^= *buf++;
        for (int i = 0; i < 8; i++) crc = (crc & 1) ? (crc >> 1) ^ 0xA001 : (crc >> 1);
    }
    return crc;
}

uint16_t crc16(const std::string& data)
{
    return crc16_update(0xFFFF, reinterpret_cast<const unsigned char*>(data.c_str()), data.size());
}

uint16_t crc16_file(const std::string& filepath)
{
    auto file = open_file(filepath);
    if (!file) return 0;

    uint16_t crc = 0xFFFF;
    auto buffer = make_buffer();
    while (file.good())
    {
        file.read(buffer.data(), buffer.size());
        crc = crc16_update(crc, reinterpret_cast<const unsigned char*>(buffer.data()), file.gcount());
    }
    return crc;
}

// ---------------- CRC32 ----------------
static uint32_t crc32_update(uint32_t crc, const unsigned char* buf, size_t len)
{
    crc = ~crc;
    while (len--)
    {
        crc ^= *buf++;
        for (int i = 0; i < 8; i++) crc = (crc & 1) ? (crc >> 1) ^ 0xEDB88320 : (crc >> 1);
    }
    return ~crc;
}

uint32_t crc32(const std::string& data)
{
    return crc32_update(0, reinterpret_cast<const unsigned char*>(data.c_str()), data.size());
}

uint32_t crc32_file(const std::string& filepath)
{
    auto file = open_file(filepath);
    if (!file) return 0;

    uint32_t crc = 0;
    auto buffer = make_buffer();
    while (file.good())
    {
        file.read(buffer.data(), buffer.size());
        crc = crc32_update(crc, reinterpret_cast<const unsigned char*>(buffer.data()), file.gcount());
    }
    return crc;
}

// ---------------- 和校验 ----------------
uint8_t sum8(const std::string& data)
{
    uint16_t sum = 0;
    for (auto c : data) sum += static_cast<uint8_t>(c);
    return static_cast<uint8_t>(sum & 0xFF);
}

uint8_t sum8_file(const std::string& filepath)
{
    auto file = open_file(filepath);
    if (!file) return 0;

    uint16_t sum = 0;
    auto buffer = make_buffer();
    while (file.good())
    {
        file.read(buffer.data(), buffer.size());
        for (std::streamsize i = 0; i < file.gcount(); ++i) sum += static_cast<uint8_t>(buffer[i]);
    }
    return static_cast<uint8_t>(sum & 0xFF);
}

uint16_t sum16(const std::string& data)
{
    uint32_t sum = 0;
    for (auto c : data) sum += static_cast<uint8_t>(c);
    return static_cast<uint16_t>(sum & 0xFFFF);
}

uint16_t sum16_file(const std::string& filepath)
{
    auto file = open_file(filepath);
    if (!file) return 0;

    uint32_t sum = 0;
    auto buffer = make_buffer();
    while (file.good())
    {
        file.read(buffer.data(), buffer.size());
        for (std::streamsize i = 0; i < file.gcount(); ++i) sum += static_cast<uint8_t>(buffer[i]);
    }
    return static_cast<uint16_t>(sum & 0xFFFF);
}

// ---------------- 异或校验 ----------------
uint8_t xor8(const std::string& data)
{
    uint8_t val = 0;
    for (auto c : data) val ^= static_cast<uint8_t>(c);
    return val;
}

uint8_t xor8_file(const std::string& filepath)
{
    auto file = open_file(filepath);
    if (!file) return 0;

    uint8_t val = 0;
    auto buffer = make_buffer();
    while (file.good())
    {
        file.read(buffer.data(), buffer.size());
        for (std::streamsize i = 0; i < file.gcount(); ++i) val ^= static_cast<uint8_t>(buffer[i]);
    }
    return val;
}

// ---------------- LRC ----------------
uint8_t lrc8(const std::string& data)
{
    uint8_t sum = 0;
    for (auto c : data) sum += static_cast<uint8_t>(c);
    return static_cast<uint8_t>(-sum);
}

uint8_t lrc8_file(const std::string& filepath)
{
    auto file = open_file(filepath);
    if (!file) return 0;

    uint8_t sum = 0;
    auto buffer = make_buffer();
    while (file.good())
    {
        file.read(buffer.data(), buffer.size());
        for (std::streamsize i = 0; i < file.gcount(); ++i) sum += static_cast<uint8_t>(buffer[i]);
    }
    return static_cast<uint8_t>(-sum);
}

// ---------------- Fletcher16 ----------------
uint16_t fletcher16(const std::string& data)
{
    uint16_t sum1 = 0;
    uint16_t sum2 = 0;
    for (auto c : data)
    {
        sum1 = (sum1 + static_cast<uint8_t>(c)) % 255;
        sum2 = (sum2 + sum1) % 255;
    }
    return (sum2 << 8) | sum1;
}

uint16_t fletcher16_file(const std::string& filepath)
{
    auto file = open_file(filepath);
    if (!file) return 0;

    uint16_t sum1 = 0;
    uint16_t sum2 = 0;
    auto buffer = make_buffer();
    while (file.good())
    {
        file.read(buffer.data(), buffer.size());
        for (std::streamsize i = 0; i < file.gcount(); ++i)
        {
            sum1 = (sum1 + static_cast<uint8_t>(buffer[i])) % 255;
            sum2 = (sum2 + sum1) % 255;
        }
    }
    return (sum2 << 8) | sum1;
}

// ---------------- Fletcher32 ----------------
uint32_t fletcher32(const std::string& data)
{
    uint32_t sum1 = 0;
    uint32_t sum2 = 0;
    for (auto c : data)
    {
        sum1 = (sum1 + static_cast<uint8_t>(c)) % 65535;
        sum2 = (sum2 + sum1) % 65535;
    }
    return (sum2 << 16) | sum1;
}

uint32_t fletcher32_file(const std::string& filepath)
{
    auto file = open_file(filepath);
    if (!file) return 0;

    uint32_t sum1 = 0;
    uint32_t sum2 = 0;
    auto buffer = make_buffer();
    while (file.good())
    {
        file.read(buffer.data(), buffer.size());
        for (std::streamsize i = 0; i < file.gcount(); ++i)
        {
            sum1 = (sum1 + static_cast<uint8_t>(buffer[i])) % 65535;
            sum2 = (sum2 + sum1) % 65535;
        }
    }
    return (sum2 << 16) | sum1;
}

}  // namespace checkutils
