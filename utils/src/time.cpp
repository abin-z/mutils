#include "utils/time.h"

#include <chrono>
#include <iomanip>
#include <sstream>

namespace timeutils
{

// ---------------- 获取当前时间字符串 ----------------
std::string get_now_time_string(const std::string &fmt_str)
{
  using namespace std::chrono;

  auto now = system_clock::now();
  auto t = system_clock::to_time_t(now);
  std::tm tm;

#ifdef _WIN32
  localtime_s(&tm, &t);
#else
  localtime_r(&t, &tm);
#endif

  std::ostringstream oss;
  std::string fmt_copy = fmt_str.empty() ? "%Y-%m-%d %H:%M:%S" : fmt_str;

  // 处理 %f 毫秒
  size_t pos = fmt_copy.find("%f");
  if (pos != std::string::npos)
  {
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // 将 %f 替换为占位符
    fmt_copy.replace(pos, 2, "%03d");

    char buf[128];
    std::strftime(buf, sizeof(buf), fmt_copy.c_str(), &tm);
    oss << buf;
    oss << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
  }

  // 普通 strftime 格式
  char buf[128];
  std::strftime(buf, sizeof(buf), fmt_copy.c_str(), &tm);
  return std::string(buf);
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
