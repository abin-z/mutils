#include <fmt/core.h>

#include <cassert>
#include <string>

#include "utils/make_unique.h"

struct Foo
{
  Foo(int a, std::string b) : x(a), y(std::move(b))
  {
    fmt::print("Foo::Foo({}, {})\n", x, y);
  }

  ~Foo()
  {
    fmt::print("Foo::~Foo()\n");
  }

  int x;
  std::string y;
};

static void test_basic_make_unique()
{
  auto p = utils::make_unique<Foo>(42, "hello");

  assert(p);
  assert(p->x == 42);
  assert(p->y == "hello");

  fmt::print("[basic] make_unique works\n");
}

static void test_move_only()
{
  auto p = utils::make_unique<std::unique_ptr<int>>(new int(123));

  assert(p);
  assert(**p == 123);

  fmt::print("[move-only] ok\n");
}

static void test_reset_and_destroy()
{
  auto p = utils::make_unique<Foo>(1, "destroy-test");
  p.reset();  // 触发析构

  fmt::print("[lifetime] reset ok\n");
}

int main()
{
  try
  {
    test_basic_make_unique();
    test_move_only();
    test_reset_and_destroy();

    fmt::print("\nAll make_unique tests passed ✔️\n");
  }
  catch (const std::exception &e)
  {
    fmt::print(stderr, "Test failed with exception: {}\n", e.what());
    return 1;
  }
  catch (...)
  {
    fmt::print(stderr, "Test failed with unknown exception\n");
    return 1;
  }

  return 0;
}
