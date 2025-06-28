#pragma once
#include <Common.h>
#include <iostream>

namespace cppQuiz {
// Quiz1
// 主模板，可以放在头文件中定义
template <class T> void quiz1_1(T &i) { PRINT_FUNC_NAME(); }
// 显示特化模板，相当于普通函数，若头文件中定义，会被多个编译单元包含导致重复定义
template <> void quiz1_1(const int &i);

// 主模板，可以放在头文件中定义
template <class T> void quiz1_2(T &i) { PRINT_FUNC_NAME(); }
// 显式特化模板（添加inline），inline允许函数在多个编译单元中重复定义，连接器会合并他们
template <> inline void quiz1_2(const int &i) { PRINT_FUNC_NAME(); }
void cppQuiz1();

// quiz2
void quiz2(const std::string &);
void quiz2(const char *);
void quiz2(const void *);

void cppQuiz2();

// quiz29
struct quiz29A {
  quiz29A() { foo(); }
  virtual ~quiz29A() { foo(); }
  virtual void foo() { std::cout << "1"; }
  void bar() { foo(); }
};

struct quiz29B : public quiz29A {
  virtual void foo() { std::cout << "2"; }
};

void cppQuiz29();

// quiz31

struct Quiz31X {
  Quiz31X() { std::cout << "Quiz31X"; }
};

struct Quiz31Y {
  Quiz31Y(const Quiz31X &x) { std::cout << "Y"; }
  void f() { std::cout << "f"; }
};

void cppQuiz31();

void cppQuiz38();

struct cppQuiz42A {
  cppQuiz42A() { std::cout << "1"; }

  cppQuiz42A(int) { std::cout << "2"; }

  cppQuiz42A(std::initializer_list<int>) { std::cout << "3"; }
};

void cppQuiz42();


void run();
} // namespace cppQuiz