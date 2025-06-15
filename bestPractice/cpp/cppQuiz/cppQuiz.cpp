#include "cppQuiz.hpp"

namespace cppQuiz {

// 显示特化模板
// 放在头文件中时，会被多个包含该头文件的源文件编译，导致重复定义
template <> void quiz1_1(const int &i) { PRINT_FUNC_NAME(); }

void cppQuiz1() {
  PRINT_FUNC_NAME(); // i 是 int 类型的非常量左值
  int i = 42;

  // 主模板是精确匹配、特化模板需要类型转换（int&->const int&
  // C++重载规则，优先选择不需要类型转换的精确匹配
  quiz1_1(i);
  // 核心结论
  // 1.模板特化不参与重载决议：编译器先选择最匹配的模板，再检查是否有特化
  // 2.常量性影响匹配：const 修饰符会导致不同的函数签名
  // 3.精确匹配优先：C++ 总是优先选择需要最少隐式转换的版本
  quiz1_2(i);
}

void quiz2(const std::string &) { PRINT_FUNC_NAME(); }
void quiz2(const char *) { PRINT_FUNC_NAME(); }
void quiz2(const void *) { PRINT_FUNC_NAME(); }

void cppQuiz2() {
  PRINT_FUNC_NAME();
  // 字符串标量并不是 std::string ,而是 const char[],若要匹配quiz_2(const
  // string&),必须经历一次到string的隐式转换
  quiz2("foo");
  const char *bar = "bar";
  quiz2(bar);
  const void *bar2 = "bar2";
  quiz2(bar2);
  quiz2(std::string("string"));
}

void run() {
  cppQuiz1();
  cppQuiz2();
}
} // namespace cppQuiz
