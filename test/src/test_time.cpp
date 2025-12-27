#include <catch2/catch.hpp>
#include <chrono>
#include <regex>
#include <string>
#include <thread>

#include "utils/time.h"

using namespace timeutils;

// ---------------- get_now_time_string 测试 ----------------
TEST_CASE("timeutils: get_now_time_string returns non-empty string", "[timeutils][string]")
{
  std::string t1 = get_now_time_string();
  REQUIRE(!t1.empty());

  std::string t2 = get_now_time_string("%Y-%m-%d");
  REQUIRE(!t2.empty());
  REQUIRE(t2.size() >= 10);  // 年-月-日长度至少 10
}

// ---------------- 时间戳基本合法性 ----------------
TEST_CASE("timeutils: current_timestamp_sec and millis are reasonable", "[timeutils][timestamp]")
{
  int64_t sec1 = current_timestamp_sec();
  int64_t sec2 = current_timestamp_sec();
  REQUIRE(sec2 >= sec1);  // 时间单调递增或相等

  int64_t ms1 = current_timestamp_millis();
  int64_t ms2 = current_timestamp_millis();
  REQUIRE(ms2 >= ms1);  // 毫秒单调递增或相等

  // 秒级和毫秒级大致对应
  REQUIRE(ms1 / 1000 <= sec2);
  REQUIRE(sec1 * 1000 <= ms2);
}

// ---------------- 时间顺序性 ----------------
TEST_CASE("timeutils: timestamps increase over sleep", "[timeutils][order]")
{
  int64_t sec_before = current_timestamp_sec();
  int64_t ms_before = current_timestamp_millis();

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  int64_t sec_after = current_timestamp_sec();
  int64_t ms_after = current_timestamp_millis();

  REQUIRE(sec_after >= sec_before);
  REQUIRE(ms_after > ms_before);
}

// ---------------- 格式化测试 ----------------
TEST_CASE("timeutils: custom format string", "[timeutils][format]")
{
  std::string fmt1 = "%Y-%m-%d %H:%M:%S.%f";
  std::string s1 = get_now_time_string(fmt1);
  REQUIRE(!s1.empty());
  REQUIRE(s1.find('.') != std::string::npos);  // 包含小数秒

  std::string fmt2 = "%H:%M:%S";
  std::string s2 = get_now_time_string(fmt2);
  REQUIRE(!s2.empty());
  REQUIRE(s2.size() >= 8);
}

// ---------------- 非空与基本格式检查 ----------------
TEST_CASE("timeutils: basic string output", "[timeutils][string]")
{
  std::string s = get_now_time_string();
  REQUIRE(!s.empty());
  REQUIRE(s.size() >= 19);  // "YYYY-MM-DD HH:MM:SS" 长度至少 19
}

// ---------------- 自定义格式 ----------------
TEST_CASE("timeutils: custom format output", "[timeutils][format]")
{
  auto s1 = get_now_time_string("%Y-%m-%d");
  REQUIRE(!s1.empty());
  REQUIRE(s1.size() == 10);

  auto s2 = get_now_time_string("%H:%M:%S");
  REQUIRE(!s2.empty());
  REQUIRE(s2.size() == 8);

  auto s3 = get_now_time_string("%Y-%m-%d %H:%M:%S.%f");
  REQUIRE(!s3.empty());
  REQUIRE(s3.find('.') != std::string::npos);
}

// ---------------- 时间戳合理性 ----------------
TEST_CASE("timeutils: timestamps reasonable", "[timeutils][timestamp]")
{
  int64_t sec = current_timestamp_sec();
  int64_t ms = current_timestamp_millis();

  REQUIRE(sec >= 0);
  REQUIRE(ms >= 0);
  REQUIRE(ms / 1000 <= sec + 1);  // 毫秒和秒差不大于1秒
}

// ---------------- 秒/毫秒连续性 ----------------
TEST_CASE("timeutils: timestamps monotonicity", "[timeutils][order]")
{
  int64_t sec1 = current_timestamp_sec();
  int64_t ms1 = current_timestamp_millis();

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  int64_t sec2 = current_timestamp_sec();
  int64_t ms2 = current_timestamp_millis();

  REQUIRE(sec2 >= sec1);
  REQUIRE(ms2 > ms1);
  REQUIRE(ms2 - ms1 <= 1000);  // 不会超过1秒
}

// ---------------- 快速连续调用 ----------------
TEST_CASE("timeutils: rapid consecutive calls", "[timeutils][fast]")
{
  int64_t ms_prev = current_timestamp_millis();
  for (int i = 0; i < 100; ++i)
  {
    int64_t ms_now = current_timestamp_millis();
    REQUIRE(ms_now >= ms_prev);
    ms_prev = ms_now;
  }
}

// ---------------- 字符串与时间戳对应 ----------------
TEST_CASE("timeutils: timestamp and string consistency", "[timeutils][consistency]")
{
  int64_t ts1 = current_timestamp_sec();
  std::string tstr = get_now_time_string("%Y-%m-%d %H:%M:%S");

  // 简单正则匹配 YYYY-MM-DD HH:MM:SS
  std::regex re("\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}");
  REQUIRE(std::regex_match(tstr, re));

  int64_t ts2 = current_timestamp_sec();
  // 当前时间字符串对应的时间戳应该在 ts1 ~ ts2 区间内
  REQUIRE(ts1 <= ts2);
}