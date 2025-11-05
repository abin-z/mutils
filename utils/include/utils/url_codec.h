#pragma once
#include <string>

namespace codec
{

// ---------------- URL 编码/解码 ----------------
std::string url_encode(const std::string& value);
std::string url_decode(const std::string& value);

}  // namespace codec
