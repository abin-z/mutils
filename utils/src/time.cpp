#include "utils/time.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace timeutils
{
namespace detail
{

// 仅允许的 strftime token
// 其余一律转义，防止 MSVC 崩溃
inline bool is_supported_token(char c)
{
  switch (c)
  {
  case 'Y':
  case 'm':
  case 'd':
  case 'H':
  case 'M':
  case 'S':
  case 'f':  // 自定义：毫秒
  case '%':  // %%
    return true;
  default:
    return false;
  }
}

// 将非法 %X 转为 %%X，确保 strftime 安全
inline std::string sanitize_strftime_format(const std::string &fmt)
{
  std::string out;
  out.reserve(fmt.size() * 2);  // 最坏情况：每个 % 都被转义

  for (size_t i = 0; i < fmt.size(); ++i)
  {
    if (fmt[i] == '%' && i + 1 < fmt.size())
    {
      char next = fmt[i + 1];
      if (is_supported_token(next))
      {
        out.push_back('%');
        out.push_back(next);
      }
      else
      {
        // 非法 token → 当普通文本输出
        out.push_back('%');
        out.push_back('%');
        out.push_back(next);
      }
      ++i;
    }
    else
    {
      out.push_back(fmt[i]);
    }
  }

  return out;
}

}  // namespace detail

// ------------------------------------------------------------
std::string format_time_string(const std::chrono::system_clock::time_point &tp, const std::string &fmt_str)
{
  using namespace std::chrono;

  std::string raw_fmt = fmt_str.empty() ? "%Y-%m-%d %H:%M:%S" : fmt_str;

  // 安全化（仍然需要，防非法 %X）
  std::string fmt = detail::sanitize_strftime_format(raw_fmt);

  // time_t → tm
  auto t = system_clock::to_time_t(tp);
  std::tm tm{};
#ifdef _WIN32
  localtime_s(&tm, &t);
#else
  localtime_r(&t, &tm);
#endif

  // 毫秒 [0,999]
  auto ms_total = duration_cast<milliseconds>(tp.time_since_epoch()).count();
  int millis = static_cast<int>(ms_total % 1000);
  if (millis < 0) millis += 1000;

  std::ostringstream out;
  out << std::setfill('0');

  size_t pos = 0;
  while (true)
  {
    size_t fpos = fmt.find("%f", pos);

    // 没有 %f：剩余部分全部交给 strftime
    if (fpos == std::string::npos)
    {
      if (pos < fmt.size())
      {
        char buf[128]{};
        std::strftime(buf, sizeof(buf), fmt.substr(pos).c_str(), &tm);
        out << buf;
      }
      break;
    }

    // %f 前的部分
    if (fpos > pos)
    {
      char buf[128]{};
      std::strftime(buf, sizeof(buf), fmt.substr(pos, fpos - pos).c_str(), &tm);
      out << buf;
    }

    // 一个 %f → 一个毫秒
    out << std::setw(3) << millis;

    pos = fpos + 2;
  }

  return out.str();
}

std::string now_time_string(const std::string &fmt_str)
{
  return format_time_string(std::chrono::system_clock::now(), fmt_str);
}

int64_t current_timestamp_sec()
{
  using namespace std::chrono;
  return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
}

int64_t current_timestamp_millis()
{
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

}  // namespace timeutils
