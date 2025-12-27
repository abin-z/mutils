#include "utils/time.h"

#include <chrono>
#include <iomanip>
#include <sstream>

namespace timeutils
{

std::string format_time_point(const std::chrono::system_clock::time_point &tp, const std::string &fmt_str)
{
  using namespace std::chrono;

  auto t = system_clock::to_time_t(tp);
  std::tm tm;

#ifdef _WIN32
  localtime_s(&tm, &t);
#else
  localtime_r(&t, &tm);
#endif

  std::string fmt = fmt_str.empty() ? "%Y-%m-%d %H:%M:%S" : fmt_str;

  // 计算毫秒，保证 [0, 999]
  auto ms_total = duration_cast<milliseconds>(tp.time_since_epoch()).count();
  int millis = static_cast<int>(ms_total % 1000);
  if (millis < 0) millis += 1000;
  // 处理 %f 占位符
  auto pos = fmt.find("%f");
  if (pos != std::string::npos)
  {
    std::string left = fmt.substr(0, pos);
    std::string right = fmt.substr(pos + 2);

    char buf[128]{};
    std::string result;

    if (!left.empty())
    {
      std::strftime(buf, sizeof(buf), left.c_str(), &tm);
      result += buf;
    }

    // 拼毫秒
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(3) << millis;
    result += oss.str();

    if (!right.empty())
    {
      std::strftime(buf, sizeof(buf), right.c_str(), &tm);
      result += buf;
    }

    return result;
  }

  char buf[128]{};
  std::strftime(buf, sizeof(buf), fmt.c_str(), &tm);
  return std::string(buf);
}

std::string now_time_string(const std::string &fmt_str)
{
  return format_time_point(std::chrono::system_clock::now(), fmt_str);
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
