#include "cpp/SmartPointTest.hpp"
#include "cpp/cppQuiz/cppQuiz.hpp"
#include "cpp/cppWeekly/cppWeekly.h"
#include "design/pimpl/pimplTest.h"
#include <iostream>

int main() {
  // test arch design
  {
    std::cout << "test arch design" << std::endl;
    design::pimplTest pimplTest;
    pimplTest.process();
  }

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