#include <fmt/core.h>

#include "utils/time.h"

int main()
{
  // 5. 时间戳
  fmt::print("timestamp (sec):    {}\n", timeutils::current_timestamp_sec());

  fmt::print("timestamp (millis): {}\n", timeutils::current_timestamp_millis());

  return 0;
}
