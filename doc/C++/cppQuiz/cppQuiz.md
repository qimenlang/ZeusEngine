

“C++ Quiz 不是测试你知道什么，而是揭示你不知道什么。” —— Anders Schau Knatten

[C++Quiz](https://cppquiz.org/quiz/question/1)


15:
#include <iostream>
#include <exception>

int x = 0;
class AA {
public:
  AA() {
    std::cout << "dd";
  }
  ~AA() { std::cout << "DD"; }
};


class A : public AA {
public:
  A() {
    std::cout << 'a';
    if (x++ == 0) {
      throw std::exception();
    }
  }
  ~A() { std::cout << 'A'; }
};

class B {
public:
  B() { std::cout << 'b'; }
  ~B() { std::cout << 'B'; }
  A a;
};

void foo() { static B b; }

int main() {
  try {
    foo();
  }
  catch (std::exception &) {
    std::cout << 'c';
    foo();
  }
}

✅ 自动清理机制：

    当构造函数抛出异常时，C++ 保证：

        已构造的基类和成员会被自动析构

        对象本身的内存会被释放