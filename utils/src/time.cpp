#include "utils/time.h"

#include <chrono>

namespace timeutils
{

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
