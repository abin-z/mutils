#include "utils/time.h"

#include <fmt/chrono.h>
#include <fmt/core.h>

#include <chrono>

namespace timeutils
{

std::string get_now_time_string(const std::string& fmt_str)
{
  using namespace std::chrono;

  auto now = system_clock::now();
  std::time_t t = system_clock::to_time_t(now);
  std::tm tm = *std::localtime(&t);

  if (fmt_str.find("%f") != std::string::npos)
  {
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    // 秒级格式化
    std::string s = fmt::format("{:%Y-%m-%d %H:%M:%S}", tm);
    // 拼接毫秒
    s += fmt::format(".{:03}", static_cast<int>(ms.count()));
    return s;
  }

  return fmt::format("{:%Y-%m-%d %H:%M:%S}", tm);
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
