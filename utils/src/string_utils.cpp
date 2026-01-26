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

char to_lower(char c)
{
  return (c >= 'A' && c <= 'Z') ? static_cast<char>(c - 'A' + 'a') : c;
}

char to_upper(char c)
{
  return (c >= 'a' && c <= 'z') ? static_cast<char>(c - 'a' + 'A') : c;
}

std::string to_lower(const std::string &str)
{
  std::string result;
  result.reserve(str.size());
  for (unsigned char c : str)
  {
    result.push_back(to_lower(static_cast<char>(c)));
  }
  return result;
}

std::string to_upper(const std::string &str)
{
  std::string result;
  result.reserve(str.size());
  for (unsigned char c : str)
  {
    result.push_back(to_upper(static_cast<char>(c)));
  }
  return result;
}

bool starts_with(const std::string &str, char c)
{
  return !str.empty() && str.front() == c;
}
bool starts_with(const std::string &str, const std::string &prefix)
{
  return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

bool ends_with(const std::string &str, char c)
{
  return !str.empty() && str.back() == c;
}
bool ends_with(const std::string &str, const std::string &suffix)
{
  return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool contains(const std::string &str, char c)
{
  return str.find(c) != std::string::npos;
}
bool contains(const std::string &str, const std::string &substr)
{
  if (substr.empty()) return true;
  return str.find(substr) != std::string::npos;
}

}  // namespace stringutils