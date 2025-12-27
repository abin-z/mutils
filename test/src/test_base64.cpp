#include <catch2/catch.hpp>
#include <string>
#include <vector>

#include "utils/base64.h"

using codec::base64_decode;
using codec::base64_encode;
using codec::base64_encode_mime;
using codec::base64_encode_pem;

TEST_CASE("base64: encode/decode basic string", "[base64]")
{
  const std::string input = "hello world";
  const std::string encoded = base64_encode(input);

  REQUIRE(encoded == "aGVsbG8gd29ybGQ=");

  const std::string decoded = base64_decode(encoded);
  REQUIRE(decoded == input);
}

TEST_CASE("base64: empty string", "[base64]")
{
  std::string input;
  REQUIRE(base64_encode(input).empty());
  REQUIRE(base64_decode(input).empty());
}

TEST_CASE("base64: binary data with null bytes", "[base64]")
{
  std::string input;
  input.push_back('\0');
  input.push_back('\x01');
  input.push_back('\xFF');
  input.push_back('\0');

  const std::string encoded = base64_encode(input);
  const std::string decoded = base64_decode(encoded);

  REQUIRE(decoded.size() == input.size());
  REQUIRE(decoded == input);
}

TEST_CASE("base64: url safe encoding", "[base64][url]")
{
  const std::string input = "https://example.com/?a=1&b=2";
  const std::string encoded = base64_encode(input, true);

  // URL-safe base64 must not contain '+' or '/'
  REQUIRE(encoded.find('+') == std::string::npos);
  REQUIRE(encoded.find('/') == std::string::npos);

  const std::string decoded = base64_decode(encoded);
  REQUIRE(decoded == input);
}

TEST_CASE("base64: PEM encoding contains line breaks", "[base64][pem]")
{
  const std::string input(100, 'A');
  const std::string encoded = base64_encode_pem(input);

  REQUIRE(encoded.find('\n') != std::string::npos);

  const std::string decoded = base64_decode(encoded, true);
  REQUIRE(decoded == input);
}

TEST_CASE("base64: MIME encoding round-trip", "[base64][mime]")
{
  const std::string input(200, 'B');
  const std::string encoded = base64_encode_mime(input);

  const std::string decoded = base64_decode(encoded, true);
  REQUIRE(decoded == input);
}

TEST_CASE("base64: unsigned char buffer interface", "[base64][buffer]")
{
  const std::vector<unsigned char> data = {0x00, 0x11, 0x22, 0x33, 0xFF};

  const std::string encoded = base64_encode(data.data(), data.size());

  const std::string decoded = base64_decode(encoded);

  REQUIRE(decoded.size() == data.size());

  for (size_t i = 0; i < data.size(); ++i)
  {
    REQUIRE(static_cast<unsigned char>(decoded[i]) == data[i]);
  }
}

TEST_CASE("base64: round-trip random-like data", "[base64][roundtrip]")
{
  std::string input;
  for (int i = 0; i < 256; ++i) input.push_back(static_cast<char>(i));

  const std::string encoded = base64_encode(input);
  const std::string decoded = base64_decode(encoded);

  REQUIRE(decoded == input);
}

TEST_CASE("base64: invalid input throws", "[base64][invalid]")
{
  const std::string invalid = "!!!!@@@@####";
  REQUIRE_THROWS(base64_decode(invalid));
}

#if __cplusplus >= 201703L
TEST_CASE("base64: string_view interface", "[base64][string_view]")
{
  std::string input = "string_view test";
  std::string_view view(input);

  const std::string encoded = base64_encode(view);
  const std::string decoded = base64_decode(encoded);

  REQUIRE(decoded == input);
}
#endif
