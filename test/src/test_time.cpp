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
  std::string t1 = now_time_string();
  REQUIRE(!t1.empty());

  std::string t2 = now_time_string("%Y-%m-%d");
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
  std::string s1 = now_time_string(fmt1);
  REQUIRE(!s1.empty());
  REQUIRE(s1.find('.') != std::string::npos);  // 包含小数秒

  std::string fmt2 = "%H:%M:%S";
  std::string s2 = now_time_string(fmt2);
  REQUIRE(!s2.empty());
  REQUIRE(s2.size() >= 8);
}

// ---------------- 非空与基本格式检查 ----------------
TEST_CASE("timeutils: basic string output", "[timeutils][string]")
{
  std::string s = now_time_string();
  REQUIRE(!s.empty());
  REQUIRE(s.size() >= 19);  // "YYYY-MM-DD HH:MM:SS" 长度至少 19
}

// ---------------- 自定义格式 ----------------
TEST_CASE("timeutils: custom format output", "[timeutils][format]")
{
  auto s1 = now_time_string("%Y-%m-%d");
  REQUIRE(!s1.empty());
  REQUIRE(s1.size() == 10);

  auto s2 = now_time_string("%H:%M:%S");
  REQUIRE(!s2.empty());
  REQUIRE(s2.size() == 8);

  auto s3 = now_time_string("%Y-%m-%d %H:%M:%S.%f");
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
  std::string tstr = now_time_string("%Y-%m-%d %H:%M:%S");

  // 简单正则匹配 YYYY-MM-DD HH:MM:SS
  std::regex re("\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}");
  REQUIRE(std::regex_match(tstr, re));

  int64_t ts2 = current_timestamp_sec();
  // 当前时间字符串对应的时间戳应该在 ts1 ~ ts2 区间内
  REQUIRE(ts1 <= ts2);
}

// ---------------- 多个 %f 出现 ----------------
TEST_CASE("timeutils: multiple %f placeholders", "[timeutils][format][millis]")
{
  std::string fmt = "%Y-%m-%d %H:%M:%S.%f.%f";
  std::string s = now_time_string(fmt);
  REQUIRE(!s.empty());

  // 检查两个小数秒拼接
  size_t first_dot = s.find('.');
  size_t second_dot = s.find('.', first_dot + 1);
  REQUIRE(first_dot != std::string::npos);
  REQUIRE(second_dot != std::string::npos);

  // 每个毫秒部分长度为3
  REQUIRE(second_dot - first_dot == 4);
}

// ---------------- 空格式字符串 ----------------
TEST_CASE("timeutils: empty format string", "[timeutils][format]")
{
  std::string s = now_time_string("");
  REQUIRE(!s.empty());  // 仍然返回非空
}

// ---------------- 极端格式测试 ----------------
TEST_CASE("timeutils: extreme format strings", "[timeutils][format]")
{
  // 只输出年份
  std::string s1 = now_time_string("%Y");
  REQUIRE(s1.size() == 4);

  // 只输出月份和日期
  std::string s2 = now_time_string("%m-%d");
  REQUIRE(s2.size() == 5);

  // 只输出小时分钟秒
  std::string s3 = now_time_string("%H:%M:%S");
  REQUIRE(s3.size() == 8);
}

// ---------------- 高频连续调用稳定性 ----------------
TEST_CASE("timeutils: high frequency calls", "[timeutils][performance]")
{
  for (int i = 0; i < 1000; ++i)
  {
    std::string s = now_time_string("%Y-%m-%d %H:%M:%S.%f");
    REQUIRE(!s.empty());
    REQUIRE(s.find('.') != std::string::npos);
  }
}

// ---------------- 格式化内容合理性 ----------------
TEST_CASE("timeutils: formatted values are reasonable", "[timeutils][format][range]")
{
  std::string s = now_time_string("%Y-%m-%d %H:%M:%S.%f");

  // 提取各部分
  int year, month, day, hour, min, sec, millis;
  int matched = sscanf(s.c_str(), "%d-%d-%d %d:%d:%d.%d", &year, &month, &day, &hour, &min, &sec, &millis);
  REQUIRE(matched == 7);

  REQUIRE((year >= 1970));
  REQUIRE((month >= 1 && month <= 12));
  REQUIRE((day >= 1 && day <= 31));
  REQUIRE((hour >= 0 && hour <= 23));
  REQUIRE((min >= 0 && min <= 59));
  REQUIRE((sec >= 0 && sec <= 59));
  REQUIRE((millis >= 0 && millis <= 999));
}

// ---------------- 特殊字符格式测试 ----------------
TEST_CASE("timeutils: special characters in format", "[timeutils][format]")
{
  std::string fmt = "Today is %Y/%m/%d @ %H-%M-%S.%f";
  std::string s = now_time_string(fmt);
  REQUIRE(!s.empty());
  REQUIRE(s.find("Today is") == 0);
  REQUIRE(s.find('@') != std::string::npos);
  REQUIRE(s.find('.') != std::string::npos);
}

// ---------------- 额外 get_now_time_string 测试 ----------------

// 空格式：应回退到默认格式
TEST_CASE("timeutils: empty format string fallback", "[timeutils][format][edge]")
{
  auto s = now_time_string("");
  REQUIRE(!s.empty());
  REQUIRE(s.size() == 19);  // YYYY-MM-DD HH:MM:SS
}

// 只包含毫秒
TEST_CASE("timeutils: format with only milliseconds", "[timeutils][format][edge]")
{
  auto s = now_time_string("%f");
  REQUIRE(s.size() == 3);

  int ms = std::stoi(s);
  REQUIRE(ms >= 0);
  REQUIRE(ms <= 999);
}

// %f 在不同位置
TEST_CASE("timeutils: %f position variants", "[timeutils][format]")
{
  auto s1 = now_time_string("%f-%H:%M:%S");
  REQUIRE(s1.size() >= 12);

  auto s2 = now_time_string("%H:%M:%S.%f");
  REQUIRE(s2.size() == 12);  // 8 + 1 + 3

  auto s3 = now_time_string("%Y-%m-%d-%f");
  REQUIRE(s3.size() == 14);  // 10 + 1 + 3
}

// 毫秒始终是 3 位
TEST_CASE("timeutils: milliseconds always three digits", "[timeutils][format]")
{
  for (int i = 0; i < 10; ++i)
  {
    auto s = now_time_string("%H:%M:%S.%f");
    auto pos = s.find('.');
    REQUIRE(pos != std::string::npos);

    std::string ms = s.substr(pos + 1);
    REQUIRE(ms.size() == 3);
  }
}

// 各字段数值范围合理
TEST_CASE("timeutils: formatted values are reasonable2", "[timeutils][format][range]")
{
  auto s = now_time_string("%Y-%m-%d %H:%M:%S.%f");

  int year = std::stoi(s.substr(0, 4));
  int month = std::stoi(s.substr(5, 2));
  int day = std::stoi(s.substr(8, 2));
  int hour = std::stoi(s.substr(11, 2));
  int minute = std::stoi(s.substr(14, 2));
  int second = std::stoi(s.substr(17, 2));
  int millis = std::stoi(s.substr(20, 3));

  REQUIRE(year >= 1970);

  REQUIRE(month >= 1);
  REQUIRE(month <= 12);

  REQUIRE(day >= 1);
  REQUIRE(day <= 31);

  REQUIRE(hour >= 0);
  REQUIRE(hour <= 23);

  REQUIRE(minute >= 0);
  REQUIRE(minute <= 59);

  REQUIRE(second >= 0);
  REQUIRE(second <= 60);  // 闰秒兼容

  REQUIRE(millis >= 0);
  REQUIRE(millis <= 999);
}

// 快速连续调用稳定性
TEST_CASE("timeutils: rapid formatted calls stability", "[timeutils][format][stress]")
{
  for (int i = 0; i < 100; ++i)
  {
    auto s = now_time_string("%Y-%m-%d %H:%M:%S.%f");
    REQUIRE(!s.empty());
    REQUIRE(s.size() == 23);  // 固定长度
  }
}

// 无 %f 时不应出现小数点
TEST_CASE("timeutils: no milliseconds no dot", "[timeutils][format]")
{
  auto s = now_time_string("%Y-%m-%d %H:%M:%S");
  REQUIRE(s.find('.') == std::string::npos);
}

// // 非法 / 未知 token 不崩溃
TEST_CASE("timeutils: unknown format tokens are safe", "[timeutils][format][robust]")
{
  auto s = now_time_string("%Q-%E-%Y");
  REQUIRE(!s.empty());
}
