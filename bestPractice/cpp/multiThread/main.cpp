#include "multiThread.h"
#include <iostream>

int main() {
  std::cout << "Multi Thread Tests!" << std::endl;
  // Run the multi-thread test
  //   multiThreadManageTest::threadCreateAndJoin();
  // multiThreadManageTest::workWithLocalPtr();
  // multiThreadManageTest::workWithLocalRef();
  multiThreadManageTest::workWithLocalRef2();
  // multiThreadManageTest::threadGuardTest();

  return 0;
}