#include <fmt/core.h>

#include <chrono>
#include <thread>

#include "utils/time.h"

int main()
{
  // 1. 当前时间（默认格式）
  fmt::print("now (default): {}\n", timeutils::now_time_string());

  // 2. 当前时间（带毫秒）
  fmt::print("now (with millis): {}\n", timeutils::now_time_string("%Y-%m-%d %H:%M:%S.%f"));

  // 3. 格式化指定 time_point
  auto tp = std::chrono::system_clock::now();
  fmt::print("formatted time_point: {}\n", timeutils::format_time_string(tp, "%Y/%m/%d %H:%M:%S"));

  // 4. 多个 %f（毫秒可重复）
  fmt::print("multiple millis: {}\n", timeutils::now_time_string("%H:%M:%S.%f.%f"));

  // 5. 时间戳
  fmt::print("timestamp (sec):    {}\n", timeutils::current_timestamp_sec());

  fmt::print("timestamp (millis): {}\n", timeutils::current_timestamp_millis());

  // 6. 时间变化演示
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  fmt::print("after sleep: {}\n", timeutils::now_time_string("%H:%M:%S.%f"));

  return 0;
}
