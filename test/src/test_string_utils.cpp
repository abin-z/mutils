#include <catch2/catch.hpp>
#include <string>
#include <vector>

#include "utils/string_utils.h"

using stringutils::split;

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
