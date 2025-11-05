#include "utils/hash.h"

#include <openssl/md5.h>
#include <openssl/sha.h>

#include <array>
#include <fstream>

namespace hashutils
{
constexpr std::size_t BUFFER_SIZE = 8192;
using Buffer = std::array<char, BUFFER_SIZE>;

// ------------------- 工具函数 -------------------
static std::string to_hex_string(const unsigned char* data, std::size_t length)
{
    static const char hex_chars[] = "0123456789abcdef";
    std::string result(length * 2, '0');
    for (size_t i = 0; i < length; ++i)
    {
        result[2 * i] = hex_chars[data[i] >> 4];       // 高4位
        result[2 * i + 1] = hex_chars[data[i] & 0xF];  // 低4位
    }
    return result;
}

static Buffer make_buffer(std::size_t size = 8192)
{
    return {};
}

// ------------------- MD5 -------------------
std::string md5(const std::string& data)
{
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), digest);
    return to_hex_string(digest, MD5_DIGEST_LENGTH);
}

std::string md5_file(const std::string& filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file) return "";

    MD5_CTX ctx;
    MD5_Init(&ctx);

    auto buffer = make_buffer();
    while (file.good())
    {
        file.read(buffer.data(), buffer.size());
        MD5_Update(&ctx, buffer.data(), file.gcount());
    }

    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_Final(digest, &ctx);
    return to_hex_string(digest, MD5_DIGEST_LENGTH);
}

// ------------------- SHA1 -------------------
std::string sha1(const std::string& data)
{
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), digest);
    return to_hex_string(digest, SHA_DIGEST_LENGTH);
}

std::string sha1_file(const std::string& filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file) return "";

    SHA_CTX ctx;
    SHA1_Init(&ctx);

    auto buffer = make_buffer();
    while (file.good())
    {
        file.read(buffer.data(), buffer.size());
        SHA1_Update(&ctx, buffer.data(), file.gcount());
    }

    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1_Final(digest, &ctx);
    return to_hex_string(digest, SHA_DIGEST_LENGTH);
}

// ------------------- SHA224 -------------------
std::string sha224(const std::string& data)
{
    unsigned char digest[SHA224_DIGEST_LENGTH];
    SHA224(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), digest);
    return to_hex_string(digest, SHA224_DIGEST_LENGTH);
}

std::string sha224_file(const std::string& filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file) return "";

    SHA256_CTX ctx;  // SHA224 用 SHA256_CTX
    SHA224_Init(&ctx);

    auto buffer = make_buffer();
    while (file.good())
    {
        file.read(buffer.data(), buffer.size());
        SHA224_Update(&ctx, buffer.data(), file.gcount());
    }

    unsigned char digest[SHA224_DIGEST_LENGTH];
    SHA224_Final(digest, &ctx);
    return to_hex_string(digest, SHA224_DIGEST_LENGTH);
}

// ------------------- SHA256 -------------------
std::string sha256(const std::string& data)
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), digest);
    return to_hex_string(digest, SHA256_DIGEST_LENGTH);
}

std::string sha256_file(const std::string& filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file) return "";

    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    auto buffer = make_buffer();
    while (file.good())
    {
        file.read(buffer.data(), buffer.size());
        SHA256_Update(&ctx, buffer.data(), file.gcount());
    }

    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_Final(digest, &ctx);
    return to_hex_string(digest, SHA256_DIGEST_LENGTH);
}

// ------------------- SHA384 -------------------
std::string sha384(const std::string& data)
{
    unsigned char digest[SHA384_DIGEST_LENGTH];
    SHA384(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), digest);
    return to_hex_string(digest, SHA384_DIGEST_LENGTH);
}

std::string sha384_file(const std::string& filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file) return "";

    SHA512_CTX ctx;  // SHA384 用 SHA512_CTX
    SHA384_Init(&ctx);

    auto buffer = make_buffer();
    while (file.good())
    {
        file.read(buffer.data(), buffer.size());
        SHA384_Update(&ctx, buffer.data(), file.gcount());
    }

    unsigned char digest[SHA384_DIGEST_LENGTH];
    SHA384_Final(digest, &ctx);
    return to_hex_string(digest, SHA384_DIGEST_LENGTH);
}

// ------------------- SHA512 -------------------
std::string sha512(const std::string& data)
{
    unsigned char digest[SHA512_DIGEST_LENGTH];
    SHA512(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), digest);
    return to_hex_string(digest, SHA512_DIGEST_LENGTH);
}

std::string sha512_file(const std::string& filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file) return "";

    SHA512_CTX ctx;
    SHA512_Init(&ctx);

    auto buffer = make_buffer();
    while (file.good())
    {
        file.read(buffer.data(), buffer.size());
        SHA512_Update(&ctx, buffer.data(), file.gcount());
    }

    unsigned char digest[SHA512_DIGEST_LENGTH];
    SHA512_Final(digest, &ctx);
    return to_hex_string(digest, SHA512_DIGEST_LENGTH);
}

}  // namespace hashutils
