#include <catch2/catch.hpp>
#include <string>

#include "utils/url_codec.h"

using namespace codec;

TEST_CASE("url_codec: basic encode/decode", "[url_codec]")
{
  std::string raw = "hello world";
  std::string encoded = url_encode(raw);
  REQUIRE(encoded != raw);  // 编码后的字符串应该不同
  std::string decoded = url_decode(encoded);
  REQUIRE(decoded == raw);
}

TEST_CASE("url_codec: reserved characters remain unchanged", "[url_codec]")
{
  std::string raw = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~";
  std::string encoded = url_encode(raw);
  REQUIRE(encoded == raw);  // 保留字符不变
  std::string decoded = url_decode(encoded);
  REQUIRE(decoded == raw);
}

TEST_CASE("url_codec: special characters", "[url_codec]")
{
  std::string raw = "hello+world@#$&";
  std::string encoded = url_encode(raw);
  REQUIRE(encoded.find(' ') == std::string::npos);  // 空格应编码
  REQUIRE(encoded.find('+') == std::string::npos);  // '+' 编码
  std::string decoded = url_decode(encoded);
  REQUIRE(decoded == raw);
}

TEST_CASE("url_codec: unicode/utf-8 characters", "[url_codec]")
{
  std::string raw = u8"测试中文/😊";
  std::string encoded = url_encode(raw);
  REQUIRE(encoded != raw);
  std::string decoded = url_decode(encoded);
  REQUIRE(decoded == raw);
}

TEST_CASE("url_codec: empty string", "[url_codec]")
{
  std::string raw;
  std::string encoded = url_encode(raw);
  REQUIRE(encoded.empty());
  std::string decoded = url_decode(encoded);
  REQUIRE(decoded.empty());
}

TEST_CASE("url_codec: round-trip consistency", "[url_codec]")
{
  std::string test_strings[] = {"hello world", "a+b=c&d=e", "1234567890", "中文测试", "!@#$%^&*()_+-=[]{}|;:',.<>/?"};

  for (const auto &s : test_strings)
  {
    REQUIRE(url_decode(url_encode(s)) == s);
  }
}
