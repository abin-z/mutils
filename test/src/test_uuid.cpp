#include <catch2/catch.hpp>
#include <regex>
#include <set>
#include <string>

#include "utils/uuid.h"

TEST_CASE("uuidv4: format and length", "[uuid]")
{
  std::string id = uuid::uuidv4();
  REQUIRE(id.size() == 36);  // 长度必须 36

  // 正则匹配 UUID v4 格式
  std::regex re("^[0-9a-f]{8}-[0-9a-f]{4}-4[0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$");
  REQUIRE(std::regex_match(id, re));
}

TEST_CASE("uuidv4: uniqueness", "[uuid][uniqueness]")
{
  const int N = 1000;
  std::set<std::string> uuids;
  for (int i = 0; i < N; ++i)
  {
    uuids.insert(uuid::uuidv4());
  }
  REQUIRE(uuids.size() == N);  // 每次生成的 UUID 都唯一
}

TEST_CASE("uuidv4: multiple calls consistency", "[uuid]")
{
  auto id1 = uuid::uuidv4();
  auto id2 = uuid::uuidv4();
  REQUIRE(id1 != id2);
}
