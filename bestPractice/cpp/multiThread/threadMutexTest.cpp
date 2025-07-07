#include "threadMutexTest.h"
#include <iostream>
#include <thread>

namespace threadMutexTest {

void threadsafeStackTest() {
  threadsafeStack<int> stack;
  auto pushStack = [](threadsafeStack<int> &stack) {
    int i = 0;
    while (i++ < 100) {
      stack.push(i);
      std::cout << "push value:" << i << ",stack size :" << stack.size()
                << std::endl;
    }
  };
  std::thread pushThread(pushStack, std::ref(stack));
  std::this_thread::sleep_for(std::chrono::seconds(1));
  pushThread.join();

  auto popStack1 = [](threadsafeStack<int> &stack) {
    while (stack.size()) {
      int value;
      stack.pop(value);
      std::cout << "pop1 value:" << value << ",stack size :" << stack.size()
                << std::endl;
    }
  };

  auto popStack2 = [](threadsafeStack<int> &stack) {
    while (stack.size()) {
      auto value = stack.pop();
      std::cout << "pop2 value:" << *value << ",stack size :" << stack.size()
                << std::endl;
    }
  };

  std::thread popThread1(popStack1, std::ref(stack));
  std::thread popThread2(popStack2, std::ref(stack));

  popThread1.join();
  popThread2.join();
}

} // namespace threadMutexTest
