// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Abin

/**
 * @file string_utils.h
 * @brief string工具类
 * @author abin
 * @date 2025-12-30
 */

#ifndef __GUARD_STRING_UTILS_H_INCLUDE_GUARD__
#define __GUARD_STRING_UTILS_H_INCLUDE_GUARD__

#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace stringutils
{
namespace detail
{
// ------------------- Trait: 是否有 begin/end -------------------
template <typename T>
class has_begin_end
{
 private:
  // 版本 1：优先匹配 test(int)
  // 内部辅助模板, 尝试调用 std::begin() 和 std::end() 来检查类型 T 是否支持它们
  // 这个测试会先尝试通过 std::begin 和 std::end 获取迭代器
  // 如果这两个函数存在, 并且能正常编译, 最后会返回 std::true_type
  template <typename U>
  static auto test(int) -> decltype(std::begin(std::declval<U &>()),  // 检查是否支持 std::begin
                                    std::end(std::declval<U &>()),    // 检查是否支持 std::end
                                    std::true_type{});                // 如果能编译成功, 返回 std::true_type
  // 版本 2：兜底，匹配其他情况
  // 如果不支持 std::begin() 或 std::end(), 会匹配到这个重载, 返回 std::false_type
  template <typename>
  static std::false_type test(...);

 public:
  // 静态常量值, 使用 decltype 和 test<T>(0) 调用来决定类型 T 是否支持 begin() 和 end()
  static constexpr bool value = decltype(test<T>(0))::value;
};

// ------------------- Trait: 是否 map 类型 -------------------
template <typename T>
class is_map
{
 private:
  template <typename U>
  static auto test(int) ->
    typename std::is_same<typename U::value_type, std::pair<const typename U::key_type, typename U::mapped_type>>::type;

  template <typename>
  static std::false_type test(...);

 public:
  static constexpr bool value = decltype(test<T>(0))::value;
};
// ------------------- Trait: 是否可 ostream 输出 -------------------
template <typename T, typename = void>
struct is_ostreamable : std::false_type
{
};
template <typename T>  // 通过 偏特化 检测
struct is_ostreamable<T, decltype(void(std::declval<std::ostream &>() << std::declval<T>()))> : std::true_type
{
};
}  // namespace detail

// ------------------- join 模板 -------------------
template <typename Iterable>
inline std::string join(const Iterable &iterable, char separator)
{
  return join(iterable, std::string(1, separator));
}

template <typename Iterable>
inline std::string join(const Iterable &iterable, const std::string &separator)
{
  // 断言 iterable 支持 begin() 和 end(), 不是 map 类型, 元素类型不是指针并且元素类型可通过 << 输出到 std::ostream
  using value_type = typename Iterable::value_type;
  static_assert(detail::has_begin_end<Iterable>::value,
                "stringutils::join() error: The type must support std::begin() and std::end()");
  static_assert(!std::is_pointer<value_type>::value,
                "stringutils::join() error: Container elements cannot be of pointer type");
  static_assert(!detail::is_map<Iterable>::value,
                "stringutils::join() error: Map types (e.g. std::map) are not supported");
  static_assert(detail::is_ostreamable<value_type>::value,
                "stringutils::join() error: Elements must be streamable to std::ostream (<<)");

  std::ostringstream oss;
  auto it = std::begin(iterable);
  auto end = std::end(iterable);
  // 处理第一个元素
  if (it != end)
  {
    oss << *it++;  // 第一个元素,随后递增迭代器
  }
  while (it != end)
  {
    oss << separator << *it++;  // 添加分割符和后续元素,随后递增迭代器
  }
  return oss.str();
}

// ------------------- split 函数 -------------------
std::vector<std::string> split(const std::string &str, char delimiter, bool skip_empty = false);
std::vector<std::string> split(const std::string &str, const std::string &delimiter, bool skip_empty = false);

// ------------------- 字符/字符串操作 -------------------
char to_lower(char c);
char to_upper(char c);
std::string to_lower(const std::string &str);
std::string to_upper(const std::string &str);
std::string trim(const std::string &str);
std::string trim_left(const std::string &str);
std::string trim_right(const std::string &str);

// ------------------- 字符串判断 -------------------
bool starts_with(const std::string &str, char c);
bool starts_with(const std::string &str, const std::string &prefix);
bool ends_with(const std::string &str, char c);
bool ends_with(const std::string &str, const std::string &suffix);
bool contains(const std::string &str, char c);
bool contains(const std::string &str, const std::string &substr);

}  // namespace stringutils

#endif  // __GUARD_STRING_UTILS_H_INCLUDE_GUARD__