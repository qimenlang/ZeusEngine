#include "multiThread.h"
#include <Common.h>
#include <iostream>
#include <thread>

namespace multiThreadManageTest {

void hello() { std::cout << "Hello from thread!" << std::endl; }

void CallableStruct::operator()() const {
  std::cout << "Hello from callable struct!" << std::endl;
}

void threadCreateAndJoin() {
  PRINT_FUNC_NAME();
  std::cout << "std::thread::hardware_concurrency():"
            << std::thread::hardware_concurrency() << std::endl;
  std::thread t1(hello);
  std::thread t2(
      []() { std::cout << "Hello from lambda thread!" << std::endl; });
  std::thread t3(CallableStruct{});
  // 在线程对象销毁之前 显式的决定join或者detach,
  // 否则程序被terminate(std::thread 析构调用std::terminate)
  t1.join();
  t2.join();
  t3.join();
}

void CallableStructWithLocalPtr::operator()() {
  // 注意：在多线程中使用外部传入的局部变量时，必须确保该变量在调用线程结束之前仍然有效
  // 否则可能会导致未定义行为
  while (*varPtr < 100) {
    std::cout << "Local variable value: " << *varPtr << std::endl;
    *varPtr++;
  }
}

void workWithLocalPtr() {
  PRINT_FUNC_NAME();
  int *localVar = new int(42);

  CallableStructWithLocalPtr callable(localVar);
  std::thread t(callable);
  // detach的线程在后台运行，无法通信、无法控制；
  t.detach();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "workWithLocalPtr End localVar :" << *localVar << std::endl;
}

void CallableStructWithLocalRef::operator()() {
  while (varRef < 100) {
    std::cout << "Local variable value: " << varRef << std::endl;
    varRef++;
  }
}
void workWithLocalRef() {
  PRINT_FUNC_NAME();
  int localVar = 42;
  CallableStructWithLocalRef callable(localVar);
  std::thread t(callable);
  // detach的线程在后台运行，无法通信、无法控制；
  t.detach();
  // std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "workWithLocalRef End localVar :" << localVar << std::endl;
}

void func(int &varRef) {
  while (varRef < 100) {
    std::cout << "func Local variable value: " << varRef << std::endl;
    varRef++;
  }
}

void workWithLocalRef2() {
  PRINT_FUNC_NAME();
  int localVar = 42;
  // std::thread 默认按值传递参数，localVar
  // 会被复制到线程的内部存储中,导致引用失效 std::thread t(func, localVar);
  // std::thread t(func, localVar);

  // std::ref(localVar) 将 localVar 包装成引用包装器,线程内部会正确将 varRef
  // 绑定到原始的 localVar
  std::thread t(func, std::ref(localVar));

  t.join();
  std::cout << "workWithLocalRef2 not changing localVar: " << localVar
            << std::endl;
}

void threadGuardTest() {
  PRINT_FUNC_NAME();

  std::thread t([]() { std::cout << "guard thread Test lambda"; });
  ThreadGuard guard(t);
  // 当guard对象析构时，会自动调用t.join()
  // 无需手动调用join或detach
  std::cout << "Thread guard test completed." << std::endl;
}

} // namespace multiThreadManageTest