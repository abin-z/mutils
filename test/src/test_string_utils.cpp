#include <array>
#include <catch2/catch.hpp>
#include <deque>
#include <forward_list>
#include <list>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "utils/string_utils.h"

using stringutils::contains;
using stringutils::ends_with;
using stringutils::split;
using stringutils::starts_with;

using stringutils::join;
using stringutils::trim;
using stringutils::trim_left;
using stringutils::trim_right;

TEST_CASE("split - basic single character delimiter", "[string][split]")
{
  SECTION("normal split")
  {
    auto r = split("a,b,c", ',', true);
    REQUIRE(r.size() == 3);
    CHECK(r[0] == "a");
    CHECK(r[1] == "b");
    CHECK(r[2] == "c");
  }

  SECTION("no delimiter found")
  {
    auto r = split("abc", ',', true);
    REQUIRE(r.size() == 1);
    CHECK(r[0] == "abc");
  }
}

TEST_CASE("split - skip_empty behavior", "[string][split]")
{
  SECTION("skip_empty = true")
  {
    auto r = split("a,,b,", ',', true);
    REQUIRE(r.size() == 2);
    CHECK(r[0] == "a");
    CHECK(r[1] == "b");
  }

  SECTION("skip_empty = false")
  {
    auto r = split("a,,b,", ',', false);
    REQUIRE(r.size() == 4);
    CHECK(r[0] == "a");
    CHECK(r[1] == "");
    CHECK(r[2] == "b");
    CHECK(r[3] == "");
  }
}

TEST_CASE("split - multi-character delimiter", "[string][split]")
{
  SECTION("basic multi-character delimiter")
  {
    auto r = split("one--two--three", "--", true);
    REQUIRE(r.size() == 3);
    CHECK(r[0] == "one");
    CHECK(r[1] == "two");
    CHECK(r[2] == "three");
  }

  SECTION("consecutive delimiters")
  {
    auto r = split("one----two", "--", false);
    REQUIRE(r.size() == 3);
    CHECK(r[0] == "one");
    CHECK(r[1] == "");
    CHECK(r[2] == "two");
  }
}

TEST_CASE("split - empty input string", "[string][split]")
{
  SECTION("empty string, skip_empty = true")
  {
    auto r = split("", ',', true);
    CHECK(r.empty());
  }

  SECTION("empty string, skip_empty = false")
  {
    auto r = split("", ',', false);
    REQUIRE(r.size() == 1);
    CHECK(r[0].empty());
  }
}

TEST_CASE("split - empty delimiter", "[string][split]")
{
  SECTION("empty delimiter, non-empty string")
  {
    auto r = split("abc", "", true);
    REQUIRE(r.size() == 1);
    CHECK(r[0] == "abc");
  }

  SECTION("empty delimiter, empty string, skip_empty = true")
  {
    auto r = split("", "", true);
    CHECK(r.empty());
  }

  SECTION("empty delimiter, empty string, skip_empty = false")
  {
    auto r = split("", "", false);
    REQUIRE(r.size() == 1);
    CHECK(r[0].empty());
  }
}

TEST_CASE("split - leading and trailing delimiters", "[string][split]")
{
  SECTION("leading delimiter")
  {
    auto r = split(",a,b", ',', false);
    REQUIRE(r.size() == 3);
    CHECK(r[0] == "");
    CHECK(r[1] == "a");
    CHECK(r[2] == "b");
  }

  SECTION("trailing delimiter")
  {
    auto r = split("a,b,", ',', false);
    REQUIRE(r.size() == 3);
    CHECK(r[0] == "a");
    CHECK(r[1] == "b");
    CHECK(r[2] == "");
  }

  SECTION("only delimiters, skip_empty = true")
  {
    auto r = split(",,,", ',', true);
    CHECK(r.empty());
  }

  SECTION("only delimiters, skip_empty = false")
  {
    auto r = split(",,,", ',', false);
    REQUIRE(r.size() == 4);
    CHECK(r[0] == "");
    CHECK(r[1] == "");
    CHECK(r[2] == "");
    CHECK(r[3] == "");
  }
}

TEST_CASE("split - multi-character delimiter edge cases", "[string][split]")
{
  SECTION("delimiter longer than content")
  {
    auto r = split("abc", "abcdef", true);
    REQUIRE(r.size() == 1);
    CHECK(r[0] == "abc");
  }

  SECTION("delimiter appears partially")
  {
    auto r = split("ab--cd-", "--", true);
    REQUIRE(r.size() == 2);
    CHECK(r[0] == "ab");
    CHECK(r[1] == "cd-");
  }

  SECTION("overlapping-like pattern (non-overlapping find)")
  {
    auto r = split("aaaa", "aa", false);
    REQUIRE(r.size() == 3);
    CHECK(r[0] == "");
    CHECK(r[1] == "");
    CHECK(r[2] == "");
  }
}

TEST_CASE("split - empty string combinations", "[string][split]")
{
  SECTION("empty string, non-empty delimiter, skip_empty = true")
  {
    auto r = split("", "--", true);
    CHECK(r.empty());
  }

  SECTION("empty string, non-empty delimiter, skip_empty = false")
  {
    auto r = split("", "--", false);
    REQUIRE(r.size() == 1);
    CHECK(r[0].empty());
  }

  SECTION("empty string, empty delimiter, skip_empty = true")
  {
    auto r = split("", "", true);
    CHECK(r.empty());
  }

  SECTION("empty string, empty delimiter, skip_empty = false")
  {
    auto r = split("", "", false);
    REQUIRE(r.size() == 1);
    CHECK(r[0].empty());
  }
}

TEST_CASE("split - special characters", "[string][split]")
{
  SECTION("whitespace delimiter")
  {
    auto r = split("a b  c", ' ', true);
    REQUIRE(r.size() == 3);
    CHECK(r[0] == "a");
    CHECK(r[1] == "b");
    CHECK(r[2] == "c");
  }

  SECTION("tab delimiter")
  {
    auto r = split("a\tb\t\tc", '\t', false);
    REQUIRE(r.size() == 4);
    CHECK(r[0] == "a");
    CHECK(r[1] == "b");
    CHECK(r[2] == "");
    CHECK(r[3] == "c");
  }

  SECTION("newline delimiter")
  {
    auto r = split("a\nb\nc", '\n', true);
    REQUIRE(r.size() == 3);
    CHECK(r[0] == "a");
    CHECK(r[1] == "b");
    CHECK(r[2] == "c");
  }
}

TEST_CASE("split - real world like strings", "[string][split]")
{
  SECTION("file path")
  {
    auto r = split("/usr/local/bin", "/", true);
    REQUIRE(r.size() == 3);
    CHECK(r[0] == "usr");
    CHECK(r[1] == "local");
    CHECK(r[2] == "bin");
  }

  SECTION("URL query")
  {
    auto r = split("a=1&b=2&c=3", "&", true);
    REQUIRE(r.size() == 3);
    CHECK(r[0] == "a=1");
    CHECK(r[1] == "b=2");
    CHECK(r[2] == "c=3");
  }

  SECTION("CSV-like line")
  {
    auto r = split("1,,3,4", ",", false);
    REQUIRE(r.size() == 4);
    CHECK(r[0] == "1");
    CHECK(r[1] == "");
    CHECK(r[2] == "3");
    CHECK(r[3] == "4");
  }
}

TEST_CASE("split - contract tests", "[string][split][contract]")
{
  SECTION("result never contains delimiter")
  {
    auto r = split("a--b--c", "--", false);
    for (const auto &s : r)
    {
      CHECK(s.find("--") == std::string::npos);
    }
  }

  SECTION("join back preserves original when skip_empty = false")
  {
    const std::string original = "a,,b,";
    auto r = split(original, ",", false);

    std::string joined;
    for (size_t i = 0; i < r.size(); ++i)
    {
      if (i > 0) joined += ",";
      joined += r[i];
    }

    CHECK(joined == original);
  }
}

TEST_CASE("to_lower / to_upper - basic ascii", "[string][case]")
{
  SECTION("all upper to lower")
  {
    CHECK(stringutils::to_lower("ABC") == "abc");
  }

  SECTION("all lower to upper")
  {
    CHECK(stringutils::to_upper("abc") == "ABC");
  }

  SECTION("mixed case")
  {
    CHECK(stringutils::to_lower("AbC") == "abc");
    CHECK(stringutils::to_upper("aBc") == "ABC");
  }
}
TEST_CASE("to_lower / to_upper - non alphabet characters preserved", "[string][case]")
{
  const std::string input = "1234-_=+!@#";

  CHECK(stringutils::to_lower(input) == input);
  CHECK(stringutils::to_upper(input) == input);
}
TEST_CASE("to_lower / to_upper - idempotent behavior", "[string][case]")
{
  const std::string lower = "already lower";
  const std::string upper = "ALREADY UPPER";

  CHECK(stringutils::to_lower(lower) == lower);
  CHECK(stringutils::to_upper(upper) == upper);
}
TEST_CASE("to_lower / to_upper - mixed content", "[string][case]")
{
  CHECK(stringutils::to_lower("UserID_ABC123") == "userid_abc123");
  CHECK(stringutils::to_upper("UserID_abc123") == "USERID_ABC123");
}
TEST_CASE("to_lower / to_upper - non ascii preserved", "[string][case]")
{
  const std::string utf8 = "中文ÄÖÜéABC";

  CHECK(stringutils::to_lower(utf8) == "中文ÄÖÜéabc");
  CHECK(stringutils::to_upper(utf8) == "中文ÄÖÜéABC");
}
TEST_CASE("to_lower / to_upper - empty string", "[string][case]")
{
  const std::string empty;

  CHECK(stringutils::to_lower(empty).empty());
  CHECK(stringutils::to_upper(empty).empty());
  CHECK(stringutils::to_lower("").empty());
  CHECK(stringutils::to_upper("").empty());
}
TEST_CASE("to_lower / to_upper - single character cases", "[string][case]")
{
  CHECK(stringutils::to_lower("A") == "a");
  CHECK(stringutils::to_upper("a") == "A");

  CHECK(stringutils::to_lower("Z") == "z");
  CHECK(stringutils::to_upper("z") == "Z");

  CHECK(stringutils::to_lower("0") == "0");
  CHECK(stringutils::to_upper("0") == "0");
}
TEST_CASE("to_lower / to_upper - control characters preserved", "[string][case]")
{
  std::string input;
  input.push_back('\n');
  input.push_back('\t');
  input.push_back('A');

  CHECK(stringutils::to_lower(input) == std::string("\n\ta"));
  CHECK(stringutils::to_upper(input) == std::string("\n\tA"));
}
TEST_CASE("to_lower / to_upper - length preserved", "[string][case][contract]")
{
  const std::string input = "AbC123!@#中文";

  CHECK(stringutils::to_lower(input).size() == input.size());
  CHECK(stringutils::to_upper(input).size() == input.size());
}
TEST_CASE("to_lower / to_upper - japanese only", "[string][case][utf8]")
{
  const std::string jp = "日本語かなカナ";

  CHECK(stringutils::to_lower(jp) == jp);
  CHECK(stringutils::to_upper(jp) == jp);
  const std::string input = "日本語ABCかなXYZ";

  CHECK(stringutils::to_lower(input) == "日本語abcかなxyz");
  CHECK(stringutils::to_upper(input) == "日本語ABCかなXYZ");
}
TEST_CASE("to_lower / to_upper - japanese mixed with symbols", "[string][case][utf8]")
{
  const std::string input = "ユーザーID_ABC123_日本";

  CHECK(stringutils::to_lower(input) == "ユーザーid_abc123_日本");
  CHECK(stringutils::to_upper(input) == "ユーザーID_ABC123_日本");
}
TEST_CASE("to_lower / to_upper - japanese utf8 integrity", "[string][case][utf8][contract]")
{
  const std::string input = "日本語テストABC";

  auto lower = stringutils::to_lower(input);
  auto upper = stringutils::to_upper(input);

  // 长度必须保持一致（UTF-8 字节级安全）
  CHECK(lower.size() == input.size());
  CHECK(upper.size() == input.size());

  // 日文子串必须完整保留
  CHECK(lower.find("日本語テスト") != std::string::npos);
  CHECK(upper.find("日本語テスト") != std::string::npos);
}

TEST_CASE("starts_with char", "[string][starts_with][char]")
{
  CHECK(starts_with("abc", 'a'));
  CHECK(!starts_with("abc", 'b'));
  CHECK(!starts_with("", 'a'));  // empty string
}

TEST_CASE("starts_with string", "[string][starts_with][string]")
{
  CHECK(starts_with("abc", "a"));
  CHECK(starts_with("abc", "ab"));
  CHECK(starts_with("abc", "abc"));
  CHECK(!starts_with("abc", "abcd"));
  CHECK(!starts_with("abc", "b"));
  CHECK(!starts_with("", "a"));    // empty string
  CHECK(!starts_with("", "abc"));  // empty string
  CHECK(starts_with("abc", ""));   // empty prefix -> true
}

TEST_CASE("ends_with char", "[string][ends_with][char]")
{
  CHECK(ends_with("abc", 'c'));
  CHECK(!ends_with("abc", 'b'));
  CHECK(!ends_with("", 'a'));  // empty string
}

TEST_CASE("ends_with string", "[string][ends_with][string]")
{
  CHECK(ends_with("abc", "c"));
  CHECK(ends_with("abc", "bc"));
  CHECK(ends_with("abc", "abc"));
  CHECK(!ends_with("abc", "abcd"));
  CHECK(!ends_with("abc", "b"));
  CHECK(!ends_with("", "a"));    // empty string
  CHECK(!ends_with("", "abc"));  // empty string
  CHECK(ends_with("abc", ""));   // empty suffix -> true
}

TEST_CASE("contains char", "[string][contains][char]")
{
  CHECK(contains("abc", 'a'));
  CHECK(contains("abc", 'b'));
  CHECK(contains("abc", 'c'));
  CHECK(!contains("abc", 'x'));
  CHECK(!contains("", 'a'));  // empty string
}

TEST_CASE("contains string", "[string][contains][string]")
{
  CHECK(contains("abc", "a"));
  CHECK(contains("abc", "ab"));
  CHECK(contains("abc", "bc"));
  CHECK(contains("abc", "abc"));
  CHECK(!contains("abc", "abcd"));
  CHECK(!contains("abc", "bca"));
  CHECK(!contains("", "a"));   // empty string
  CHECK(contains("", ""));     // empty substring -> true
  CHECK(contains("abc", ""));  // empty substring -> true
}

TEST_CASE("unicode / multibyte support", "[string][unicode]")
{
  std::string jp = "日本語ABCかな";
  // starts_with
  CHECK(starts_with(jp, "日"));
  CHECK(!starts_with(jp, 'A'));
  CHECK(starts_with(jp, "日本"));
  CHECK(!starts_with(jp, "ABC"));
  // ends_with
  CHECK(ends_with(jp, "な"));
  CHECK(!ends_with(jp, 'A'));
  CHECK(ends_with(jp, "かな"));
  CHECK(!ends_with(jp, "ABC"));
  // contains
  CHECK(contains(jp, "日"));
  CHECK(contains(jp, 'A'));
  CHECK(contains(jp, "日本"));
  CHECK(contains(jp, "ABC"));
  CHECK(!contains(jp, "XYZ"));
}

TEST_CASE("single character strings", "[string][edge]")
{
  CHECK(starts_with("a", 'a'));
  CHECK(ends_with("a", 'a'));
  CHECK(contains("a", 'a'));
  CHECK(!starts_with("a", 'b'));
  CHECK(!ends_with("a", 'b'));
  CHECK(!contains("a", 'b'));

  CHECK(starts_with("a", "a"));
  CHECK(ends_with("a", "a"));
  CHECK(contains("a", "a"));
  CHECK(!starts_with("a", "aa"));
  CHECK(!ends_with("a", "aa"));
  CHECK(!contains("a", "aa"));
}

TEST_CASE("symbols and digits", "[string][symbols]")
{
  std::string s = "123!@#abc";
  CHECK(starts_with(s, '1'));
  CHECK(!starts_with(s, '!'));
  CHECK(ends_with(s, 'c'));
  CHECK(!ends_with(s, '1'));
  CHECK(contains(s, '!'));
  CHECK(!contains(s, 'x'));
  CHECK(contains(s, "123"));
  CHECK(contains(s, "!@#"));
  CHECK(!contains(s, "xyz"));
}

TEST_CASE("trim_left - basic ASCII", "[trim_left]")
{
  CHECK(trim_left("   hello") == "hello");
  CHECK(trim_left("\t\tabc") == "abc");
  CHECK(trim_left("\n\r test") == "test");
  CHECK(trim_left("no_space") == "no_space");
  CHECK(trim_left("    ") == "");  // 全是空白
  CHECK(trim_left("") == "");      // 空字符串
}

TEST_CASE("trim_right - basic ASCII", "[trim_right]")
{
  CHECK(trim_right("hello   ") == "hello");
  CHECK(trim_right("abc\t\t") == "abc");
  CHECK(trim_right("test\n\r") == "test");
  CHECK(trim_right("no_space") == "no_space");
  CHECK(trim_right("    ") == "");  // 全是空白
  CHECK(trim_right("") == "");      // 空字符串
}

TEST_CASE("trim - basic ASCII", "[trim]")
{
  CHECK(trim("   hello   ") == "hello");
  CHECK(trim("\tabc\t") == "abc");
  CHECK(trim("\n\r test \r\n") == "test");
  CHECK(trim("no_space") == "no_space");
  CHECK(trim("    ") == "");  // 全是空白
  CHECK(trim("") == "");      // 空字符串
}

TEST_CASE("trim - left only", "[trim][trim_left]")
{
  CHECK(trim("   abc") == "abc");
}

TEST_CASE("trim - right only", "[trim][trim_right]")
{
  CHECK(trim("abc   ") == "abc");
}

TEST_CASE("trim - mixed whitespace", "[trim]")
{
  std::string s = " \t\n\r\f\vabc \t\n\r\f\v";
  CHECK(trim(s) == "abc");
  CHECK(trim_left(s) == "abc \t\n\r\f\v");
  CHECK(trim_right(s) == " \t\n\r\f\vabc");
}

TEST_CASE("trim - unicode / multibyte", "[trim][unicode]")
{
  CHECK(trim("  日本語  ") == "日本語");
  CHECK(trim_left("  日本語") == "日本語");
  CHECK(trim_right("日本語  ") == "日本語");

  CHECK(trim(" \t\n中文测试\n\t ") == "中文测试");
}

TEST_CASE("trim - emoji", "[trim][unicode]")
{
  CHECK(trim("   😀😃😄   ") == "😀😃😄");
  CHECK(trim_left("   😀") == "😀");
  CHECK(trim_right("😄   ") == "😄");
}

TEST_CASE("trim - empty / single char", "[trim][edge]")
{
  CHECK(trim("") == "");
  CHECK(trim_left("") == "");
  CHECK(trim_right("") == "");

  CHECK(trim("a") == "a");
  CHECK(trim_left("a") == "a");
  CHECK(trim_right("a") == "a");

  CHECK(trim(" ") == "");
  CHECK(trim_left(" ") == "");
  CHECK(trim_right(" ") == "");
}

TEST_CASE("trim - string with only whitespace", "[trim][edge]")
{
  CHECK(trim(" \t\n\r\v\f") == "");
  CHECK(trim_left(" \t\n\r\v\f") == "");
  CHECK(trim_right(" \t\n\r\v\f") == "");
}
TEST_CASE("trim - no whitespace", "[trim][edge]")
{
  CHECK(trim("abc") == "abc");
  CHECK(trim_left("abc") == "abc");
  CHECK(trim_right("abc") == "abc");
}
TEST_CASE("trim - whitespace in middle preserved", "[trim][edge]")
{
  CHECK(trim("  a b c  ") == "a b c");
  CHECK(trim_left("  a b c") == "a b c");
  CHECK(trim_right("a b c  ") == "a b c");
}
TEST_CASE("trim - length preservation", "[trim][contract]")
{
  std::string s = "   test string   ";
  size_t original_length = s.size();

  std::string trimmed = trim(s);
  CHECK(trimmed.size() <= original_length);

  std::string left_trimmed = trim_left(s);
  CHECK(left_trimmed.size() <= original_length);

  std::string right_trimmed = trim_right(s);
  CHECK(right_trimmed.size() <= original_length);
}
TEST_CASE("trim - unicode length preservation", "[trim][unicode][contract]")
{
  std::string s = "   日本語テスト中文哈哈哈   ";
  size_t original_length = s.size();

  std::string trimmed = trim(s);
  CHECK(trimmed.size() <= original_length);

  std::string left_trimmed = trim_left(s);
  CHECK(left_trimmed.size() <= original_length);

  std::string right_trimmed = trim_right(s);
  CHECK(right_trimmed.size() <= original_length);
}

TEST_CASE("join - vector<int>", "[join]")
{
  std::vector<int> v{1, 2, 3};
  REQUIRE(join(v, ",") == "1,2,3");
  REQUIRE(join(v, ';') == "1;2;3");

  std::vector<int> empty;
  REQUIRE(join(empty, ",") == "");

  std::vector<int> single{42};
  REQUIRE(join(single, ",") == "42");
}

TEST_CASE("join - list<int>", "[join]")
{
  std::list<int> l{4, 5, 6};
  REQUIRE(join(l, "-") == "4-5-6");

  std::list<int> empty;
  REQUIRE(join(empty, "-") == "");

  std::list<int> single{7};
  REQUIRE(join(single, "-") == "7");
}

TEST_CASE("join - deque<double>", "[join]")
{
  std::deque<double> d{1.1, 2.2, 3.3};
  REQUIRE(join(d, ", ") == "1.1, 2.2, 3.3");

  std::deque<double> single{9.9};
  REQUIRE(join(single, ", ") == "9.9");

  std::deque<double> empty;
  REQUIRE(join(empty, ", ") == "");
}

TEST_CASE("join - forward_list<char>", "[join]")
{
  std::forward_list<char> fl{'a', 'b', 'c'};
  REQUIRE(join(fl, "") == "abc");
  REQUIRE(join(fl, "-") == "a-b-c");

  std::forward_list<char> single{'z'};
  REQUIRE(join(single, "-") == "z");

  std::forward_list<char> empty;
  REQUIRE(join(empty, "-") == "");
}

TEST_CASE("join - array<std::string, 3>", "[join]")
{
  std::array<std::string, 3> arr{{"one", "two", "three"}};
  REQUIRE(join(arr, ", ") == "one, two, three");
}

TEST_CASE("join - set<int>", "[join]")
{
  std::set<int> s{3, 1, 2};
  REQUIRE(join(s, "-") == "1-2-3");  // set 自动排序

  std::set<int> single{7};
  REQUIRE(join(single, "-") == "7");

  std::set<int> empty;
  REQUIRE(join(empty, "-") == "");
}

TEST_CASE("join - unicode strings", "[join][unicode]")
{
  std::vector<std::string> v{"日本語", "テスト", "中文"};
  REQUIRE(join(v, " ") == "日本語 テスト 中文");
}

TEST_CASE("join - char vs string separator", "[join]")
{
  std::vector<int> v{10, 20, 30};
  REQUIRE(join(v, ',') == join(v, std::string(",")));
  REQUIRE(join(v, ';') == join(v, std::string(";")));
}

TEST_CASE("join - basic vector<int>", "[join]")
{
  std::vector<int> v{1, 2, 3};
  SECTION("string separator")
  {
    std::string result = join(v, ",");
    REQUIRE(result == "1,2,3");
  }
  SECTION("char separator")
  {
    std::string result = join(v, ';');
    REQUIRE(result == "1;2;3");
  }
}

TEST_CASE("join - single element container", "[join]")
{
  std::vector<std::string> v{"hello"};
  REQUIRE(join(v, ",") == "hello");
  REQUIRE(join(v, ';') == "hello");
}

TEST_CASE("join - empty container", "[join]")
{
  std::vector<int> empty;
  REQUIRE(join(empty, ",") == "");
  REQUIRE(join(empty, ';') == "");
}

TEST_CASE("join - list<string>", "[join]")
{
  std::list<std::string> l{"apple", "banana", "cherry"};
  std::string result = join(l, " | ");
  REQUIRE(result == "apple | banana | cherry");
}

TEST_CASE("join - set<double>", "[join]")
{
  std::set<double> s{3.14, 2.71, 1.41};  // set 自动排序
  std::string result = join(s, "-");
  REQUIRE(result == "1.41-2.71-3.14");
}

TEST_CASE("join - deque<char>", "[join]")
{
  std::deque<char> d{'a', 'b', 'c'};
  REQUIRE(join(d, "") == "abc");  // 空分隔符
  REQUIRE(join(d, "-") == "a-b-c");
}

TEST_CASE("join - mixed types", "[join]")
{
  std::vector<double> v{1.1, 2.2, 3.3};
  std::string result = join(v, ", ");
  REQUIRE(result == "1.1, 2.2, 3.3");
}

TEST_CASE("join - unicode strings2", "[join][unicode]")
{
  std::vector<std::string> v{"日本語", "テスト", "中文"};
  std::string result = join(v, " ");
  REQUIRE(result == "日本語 テスト 中文");
}

TEST_CASE("join - char separator vs string separator", "[join]")
{
  std::vector<int> v{10, 20, 30};
  REQUIRE(join(v, ',') == join(v, std::string(",")));
  REQUIRE(join(v, ';') == join(v, std::string(";")));
}

TEST_CASE("join - std::queue<int> via conversion", "[join][queue]")
{
  std::queue<int> q;
  q.push(10);
  q.push(20);
  q.push(30);

  // 转成 vector 进行 join
  std::vector<int> v;
  while (!q.empty())
  {
    v.push_back(q.front());
    q.pop();
  }

  std::string result = join(v, ",");
  REQUIRE(result == "10,20,30");
}

TEST_CASE("join - std::priority_queue<int> via conversion", "[join][priority_queue]")
{
  std::priority_queue<int> pq;
  pq.push(50);
  pq.push(10);
  pq.push(30);

  // priority_queue 是大顶堆，pop 顺序是从大到小
  std::vector<int> v;
  while (!pq.empty())
  {
    v.push_back(pq.top());
    pq.pop();
  }

  std::string result = join(v, "-");
  REQUIRE(result == "50-30-10");
}

TEST_CASE("join - std::queue<std::string> via conversion", "[join][queue][string]")
{
  std::queue<std::string> q;
  q.emplace("apple");
  q.emplace("banana");
  q.emplace("cherry");

  std::vector<std::string> v;
  while (!q.empty())
  {
    v.push_back(q.front());
    q.pop();
  }

  std::string result = join(v, " | ");
  REQUIRE(result == "apple | banana | cherry");
}

// TEST_CASE("join - std::map should fail", "[join][map][compile_error]") {
//     std::map<int, std::string> m{{1, "a"}, {2, "b"}};
//     join(m, ","); // static_assert triggers: Map types are not supported
// }