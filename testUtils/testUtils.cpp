#include "cpp/SmartPointTest.hpp"
#include "testPimpl/Widget.h"
#include <iostream>

int main() {
  std::cout << "test pimpl" << std::endl;
  // test Pimpl
  // {
  //   Widget widget;
  //   widget.process();
  // }

  // test cpp shared ptr thread safe
  {
    testSharedPtrThreadSafe();
  }
}