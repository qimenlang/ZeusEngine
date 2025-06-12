#include "testPimpl/Widget.h"
#include <iostream>

int main() {
  std::cout << "test pimpl" << std::endl;
  {
    Widget widget;
    widget.process();
  }

}