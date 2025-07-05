#include "multiThread.h"
#include <Common.h>
#include <iostream>
#include <thread>

namespace multiThreadTest {

void hello() { std::cout << "Hello from thread!" << std::endl; }

void CallableStruct::operator()() const {
  std::cout << "Hello from callable struct!" << std::endl;
}

void multiThreadBase() {
  PRINT_FUNC_NAME();
  std::cout << "std::thread::hardware_concurrency():"
            << std::thread::hardware_concurrency() << std::endl;
  std::thread t1(hello);
  std::thread t2(
      []() { std::cout << "Hello from lambda thread!" << std::endl; });
  std::thread t3(CallableStruct{});
  // 在线程对象销毁之前 显示的决定join
  // 或者detache,否则程序被terminate(std::thread 析构调用std::terminate)
  t1.join();
  t2.join();
  t3.join();
}

} // namespace multiThreadTest