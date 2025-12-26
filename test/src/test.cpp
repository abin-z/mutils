#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("Basic arithmetic", "[math]")
{
  REQUIRE(1 + 1 == 2);
  CHECK(2 * 3 == 6);
}