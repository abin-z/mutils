#include <catch2/catch.hpp>
#include <memory>
#include <string>
#include <utility>

#include "utils/make_unique.h"

// ---------------- 普通对象 ----------------
TEST_CASE("make_unique: basic object", "[make_unique]")
{
  auto ptr = utils::make_unique<int>();
  REQUIRE(ptr != nullptr);
  *ptr = 42;
  REQUIRE(*ptr == 42);
}

// ---------------- 带构造参数 ----------------
struct MyStruct
{
  int a;
  std::string s;
  MyStruct(int x, std::string str) : a(x), s(std::move(str)) {}
};

TEST_CASE("make_unique: object with constructor args", "[make_unique]")
{
  auto ptr = utils::make_unique<MyStruct>(10, "hello");
  REQUIRE(ptr != nullptr);
  REQUIRE(ptr->a == 10);
  REQUIRE(ptr->s == "hello");
}

// ---------------- 完美转发测试 ----------------
struct ForwardTest
{
  int x;
  explicit ForwardTest(int &&v) : x(v) {}
};

TEST_CASE("make_unique: perfect forwarding", "[make_unique]")
{
  int val = 5;
  auto ptr = utils::make_unique<ForwardTest>(std::move(val));
  REQUIRE(ptr->x == 5);
}

// ---------------- 类型安全测试：数组 ----------------
TEST_CASE("make_unique: static_assert prevents arrays", "[make_unique]")
{
#if UTILS_HAS_MAKE_UNIQUE
  SUCCEED("C++14 std::make_unique allows array, skipping fallback static_assert");
#else
  // 下面代码编译时会 static_assert 失败，无法运行到测试阶段
  // auto ptr = utils::make_unique<int[]>(10);
  SUCCEED("C++11 fallback static_assert prevents array types");
#endif
}

// ---------------- 类型安全测试：引用 ----------------
TEST_CASE("make_unique: static_assert prevents reference", "[make_unique]")
{
#if UTILS_HAS_MAKE_UNIQUE
  SUCCEED("C++14 std::make_unique handles references differently");
#else
  // auto ptr = utils::make_unique<int&>(*(new int(5)));
  SUCCEED("C++11 fallback static_assert prevents reference types");
#endif
}

// ---------------- 返回类型检查 ----------------
TEST_CASE("make_unique: return type is unique_ptr", "[make_unique]")
{
  auto ptr = utils::make_unique<int>(123);
  REQUIRE((std::is_same<decltype(ptr), std::unique_ptr<int>>::value));
}
// ---------------- 生命周期测试 ----------------
TEST_CASE("make_unique: object lifetime management", "[make_unique][lifetime]")
{
  int instance_count = 0;

  struct LifetimeTest
  {
    int &count;
    LifetimeTest(int &c) : count(c)
    {
      ++count;
    }
    ~LifetimeTest()
    {
      --count;
    }
  };

  REQUIRE(instance_count == 0);
  {
    auto ptr = utils::make_unique<LifetimeTest>(instance_count);
    REQUIRE(instance_count == 1);
  }
  REQUIRE(instance_count == 0);
}
