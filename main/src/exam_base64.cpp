#include <fmt/core.h>

#include <cassert>
#include <string>
#include <vector>

#include "utils/base64.h"

static void test_basic_string()
{
  std::string input = "hello world";
  auto encoded = codec::base64_encode(input);
  auto decoded = codec::base64_decode(encoded);

  fmt::print("[basic] encoded: {}\n", encoded);

  assert(decoded == input);
}

static void test_known_value()
{
  // RFC 4648 示例
  std::string input = "foobar";
  auto encoded = codec::base64_encode(input);

  fmt::print("[known] encoded: {}\n", encoded);

  assert(encoded == "Zm9vYmFy");
  assert(codec::base64_decode(encoded) == input);
}

static void test_binary_data()
{
  std::vector<unsigned char> data = {0x00, 0x01, 0x02, 0xFF};

  auto encoded = codec::base64_encode(data.data(), data.size());
  auto decoded = codec::base64_decode(encoded);

  fmt::print("[binary] encoded: {}\n", encoded);

  assert(decoded.size() == data.size());

  for (size_t i = 0; i < data.size(); ++i)
  {
    assert(static_cast<unsigned char>(decoded[i]) == data[i]);
  }
}

static void test_url_safe()
{
  std::string input = "hello+world/=";
  auto encoded = codec::base64_encode(input, true);
  auto decoded = codec::base64_decode(encoded);

  fmt::print("[url-safe] encoded: {}\n", encoded);

  assert(decoded == input);
}

static void test_pem_mime()
{
  std::string input(120, 'A');

  auto pem = codec::base64_encode_pem(input);
  auto mime = codec::base64_encode_mime(input);

  fmt::print("[pem]\n{}\n", pem);
  fmt::print("[mime]\n{}\n", mime);

  assert(codec::base64_decode(pem, true) == input);
  assert(codec::base64_decode(mime, true) == input);
}

#if __cplusplus >= 201703L
static void test_string_view()
{
  std::string s = "string_view test";
  std::string_view sv{s};

  auto encoded = codec::base64_encode(sv);
  auto decoded = codec::base64_decode(encoded);

  fmt::print("[string_view] encoded: {}\n", encoded);

  assert(decoded == s);
}
#endif

int main()
{
  try
  {
    test_basic_string();
    test_known_value();
    test_binary_data();
    test_url_safe();
    test_pem_mime();

#if __cplusplus >= 201703L
    test_string_view();
#endif

    fmt::print("\nAll base64 tests passed ✔️\n");
  }
  catch (const std::exception &e)
  {
    fmt::print(stderr, "Test failed with exception: {}\n", e.what());
    return 1;
  }
  catch (...)
  {
    fmt::print(stderr, "Test failed with unknown exception\n");
    return 1;
  }

  return 0;
}
