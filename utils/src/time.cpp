#include "utils/time.h"

#include <chrono>
#include <ctime>

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
std::string format_time_string(const std::chrono::system_clock::time_point &tp, const std::string &fmt_str)
{
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;
  using std::chrono::system_clock;

  std::string raw_fmt = fmt_str.empty() ? "%Y-%m-%d %H:%M:%S" : fmt_str;

  // 防 MSVC 非法 %X UB
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

  // 输出缓冲
  std::string out;
  out.reserve(fmt.size() + 8);

  // 预生成毫秒字符串
  char ms_buf[3];
  ms_buf[0] = '0' + (millis / 100) % 10;
  ms_buf[1] = '0' + (millis / 10) % 10;
  ms_buf[2] = '0' + (millis % 10);

  size_t pos = 0;
  while (true)
  {
    size_t fpos = fmt.find("%f", pos);

    // 没有 %f
    if (fpos == std::string::npos)
    {
      if (pos < fmt.size())
      {
        char buf[128]{};
        std::strftime(buf, sizeof(buf), fmt.substr(pos).c_str(), &tm);
        out += buf;
      }
      break;
    }

    // %f 前
    if (fpos > pos)
    {
      char buf[128]{};
      std::strftime(buf, sizeof(buf), fmt.substr(pos, fpos - pos).c_str(), &tm);
      out += buf;
    }

    // %f → 毫秒
    out.append(ms_buf, 3);

    pos = fpos + 2;
  }

  return out;
}

std::string now_time_string(const std::string &fmt_str)
{
  return format_time_string(std::chrono::system_clock::now(), fmt_str);
}

int64_t current_timestamp_sec()
{
  using std::chrono::duration_cast;
  using std::chrono::seconds;
  using std::chrono::system_clock;
  return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
}

int64_t current_timestamp_millis()
{
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;
  using std::chrono::system_clock;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

}  // namespace timeutils
