#include "utils/url_codec.h"

#include <cctype>
#include <stdexcept>

namespace codec
{

std::string url_encode(const std::string &value)
{
  static const char hex[] = "0123456789ABCDEF";
  std::string result;
  result.reserve(value.size() * 3);  // 最坏情况，每个字符变成 %XX

  for (unsigned char c : value)
  {
    if ((std::isalnum(c) != 0) || c == '-' || c == '_' || c == '.' || c == '~')
    {
      result.push_back(c);
    }
    else
    {
      result.push_back('%');
      result.push_back(hex[c >> 4]);
      result.push_back(hex[c & 0x0F]);
    }
  }

  return result;
}

std::string url_decode(const std::string &value)
{
  std::string result;
  result.reserve(value.size());
  size_t i = 0;

  while (i < value.size())
  {
    if (value[i] == '%' && i + 2 < value.size())
    {
      auto hex_digit = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        throw std::runtime_error("Invalid percent-encoding");
      };
      unsigned char decoded_char = (hex_digit(value[i + 1]) << 4) | hex_digit(value[i + 2]);
      result.push_back(decoded_char);
      i += 3;
    }
    else if (value[i] == '+')
    {
      result.push_back(' ');  // 可选
      ++i;
    }
    else
    {
      result.push_back(value[i]);
      ++i;
    }
  }

  return result;
}

}  // namespace codec
