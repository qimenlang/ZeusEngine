#pragma once
#include <iostream>

namespace cppQuiz {

// 主模板，可以放在头文件中定义
template <class T> void f(T &i) { std::cout << __func__ << std::endl; }
// 显示特化模板，相当于普通函数，若头文件中定义，会被多个编译单元包含导致重复定义
template <> void f(const int &i);

// 主模板，可以放在头文件中定义
template <class T> void f2(T &i) { std::cout << __func__ << std::endl; }
// 显式特化模板（添加inline），inline允许函数在多个编译单元中重复定义，连接器会合并他们
template <> inline void f2(const int &i) { std::cout << __func__ << std::endl; }

void cppQuiz1();

void run();
} // namespace cppQuiz