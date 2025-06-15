#include "cppQuiz.hpp"

namespace cppQuiz {

// 显示特化模板
// 放在头文件中时，会被多个包含该头文件的源文件编译，导致重复定义
template <> void f(const int &i) { std::cout << __func__ << std::endl; }

void cppQuiz1() {
  std::cout << __func__ << std::endl;
  // i 是 int 类型的非常量左值
  int i = 42;

  // 主模板是精确匹配、特化模板需要类型转换（int&->const int&
  // C++重载规则，优先选择不需要类型转换的精确匹配
  f(i);
  // 核心结论
  // 1.模板特化不参与重载决议：编译器先选择最匹配的模板，再检查是否有特化
  // 2.常量性影响匹配：const 修饰符会导致不同的函数签名
  // 3.精确匹配优先：C++ 总是优先选择需要最少隐式转换的版本
  f2(i);
}

void run() { cppQuiz1(); }
} // namespace cppQuiz
