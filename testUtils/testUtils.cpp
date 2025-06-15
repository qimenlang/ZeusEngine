#include "cpp/SmartPointTest.hpp"
#include "cpp/cppQuiz/cppQuiz.hpp"
#include "cpp/cppWeekly/cppWeekly.h"
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
  // {
  //   testSharedPtrThreadSafe();
  // }

  // cpp Weekly
  cppWeekly::run();
  // cpp Quiz
  cppQuiz::run();

  std::cout << "test end" << std::endl;
}