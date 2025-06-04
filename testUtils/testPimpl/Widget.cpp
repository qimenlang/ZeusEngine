#include "Widget.h"
#include <iostream>
#include <string>
#include <vector>

// 实现内部类的完整定义
struct Widget::Impl {
  std::string name;
  std::vector<int> data;
  void internalProcess() {
    std::cout << "Widget::Impl::internalProcess()" << std::endl;
  }
};

Widget::Widget() : pimpl(std::make_unique<Impl>()) {}

Widget::~Widget() = default; // 在Impl完整定义之后声明默认析构

void Widget::process() {
  pimpl->internalProcess(); // 委托给pimpl实现
}
