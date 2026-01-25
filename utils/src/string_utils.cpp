#include "utils/string_utils.h"

namespace stringutils
{
std::vector<std::string> split(const std::string &str, char delimiter, bool skip_empty)
{
  return split(str, std::string(1, delimiter), skip_empty);
}
std::vector<std::string> split(const std::string &str, const std::string &delimiter, bool skip_empty)
{
  std::vector<std::string> tokens;
  std::string::size_type start = 0;
  std::string::size_type pos = 0;
  while ((pos = str.find(delimiter, start)) != std::string::npos)
  {
    std::string token = str.substr(start, pos - start);
    if (!skip_empty || !token.empty())
    {
      tokens.emplace_back(std::move(token));
    }
    start = pos + delimiter.length();
  }
  // 处理最后一部分
  std::string last_token = str.substr(start);
  if (!skip_empty || !last_token.empty())
  {
    tokens.emplace_back(std::move(last_token));
  }
  return tokens;
}
}  // namespace stringutils