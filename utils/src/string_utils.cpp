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

  if (delimiter.empty())
  {
    if (!skip_empty || !str.empty()) tokens.emplace_back(str);
    return tokens;
  }

  std::string::size_type start = 0;
  std::string::size_type pos = 0;

  while ((pos = str.find(delimiter, start)) != std::string::npos)
  {
    if (!skip_empty || pos != start)
    {
      tokens.emplace_back(str.substr(start, pos - start));
    }
    start = pos + delimiter.length();
  }

  if (!skip_empty || start < str.size())
  {
    tokens.emplace_back(str.substr(start));
  }

  return tokens;
}

}  // namespace stringutils