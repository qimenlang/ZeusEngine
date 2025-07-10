#include "sharingDataBetweenThreads.h"
#include <iostream>
#include <thread>

namespace sharingDataBetweenThreads {

template<class U>
void swapWithDeadLock(threadsafeStack<U> &left, threadsafeStack<U> &right) {
    if (&left == &right)
      return;
    // 会因左右互换，分别锁住了两个stack的mutex,导致死锁；
    std::lock_guard lock_left(left.m_mutex);
    std::cout << std::this_thread::get_id() << " lock left :" << &left.m_mutex
              << std::endl;
    std::lock_guard lock_right(right.m_mutex);
    std::cout << std::this_thread::get_id() << " lock right :" << &right.m_mutex
              << std::endl;
    left.m_stack.swap(right.m_stack);
}

template<class U>
void swapWithoutDeakLock(threadsafeStack<U> &left,
                              threadsafeStack<U> &right) {
  if (&left == &right)
    return;
  // 同时锁住两个互斥，防止死锁
  std::lock(left.m_mutex, right.m_mutex);
  // 互斥的所有权转移到lock_guard, adopt_lock表示mutex已被锁住；
  std::lock_guard lock_left(left.m_mutex, std::adopt_lock);
  std::lock_guard lock_right(right.m_mutex, std::adopt_lock);
  left.m_stack.swap(right.m_stack);
}

template<typename U>
void swapWithScopeLock(threadsafeStack<U> &left, threadsafeStack<U> &right) {
  if (&left == &right)
    return;
  std::scoped_lock(left.m_mutex, right.m_mutex);
  left.m_stack.swap(right.m_stack);
}

template<typename U>
void swapWithUniqueLock(threadsafeStack<U> &left,  threadsafeStack<U> &right) {
  if (&left == &right)  
    return;
  // unique_lock : RAII ,获取互斥的控制权，defer_lock : 延迟锁定
  std::unique_lock<std::mutex> lock_left(left.m_mutex, std::defer_lock);
  std::unique_lock<std::mutex> lock_right(right.m_mutex, std::defer_lock);
  std::lock(lock_left, lock_right);
  left.m_stack.swap(right.m_stack);
}

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

void threadsafeStackSwapNoDeadLockTest() {
  auto initStack = [](threadsafeStack<int> &stack, int begin, int size) {
    for (int i = begin; i < begin + size; i++) {
      stack.push(i);
    }
  };
  threadsafeStack<int> stack1;
  threadsafeStack<int> stack2;
  initStack(stack1, 0, 5);
  initStack(stack2, 5, 8);

  auto swapStack = [](threadsafeStack<int> &left, threadsafeStack<int> &right,
                      int count) {
    while (count--) {
      // swapWithDeadLock(left, right);
      // swapWithoutDeakLock(left, right);
      swapWithScopeLock(left, right);
      // swapWithUniqueLock(left, right);
    }
  };

  std::thread swapThread1(swapStack, std::ref(stack1), std::ref(stack2), 100);
  std::thread swapThread2(swapStack, std::ref(stack2), std::ref(stack1), 101);

  swapThread1.join();
  swapThread2.join();
  std::cout << "stack1 size :" << stack1.size()
            << ", stack2 size :" << stack2.size() << std::endl;
}

int shared_value = 0; // 共享数据
void increment() {
  for (int i = 0; i < 100000; ++i)
    ++shared_value; // 数据竞争！
}

void dataRaceTest() {
  std::thread t1(increment);
  std::thread t2(increment);
  t1.join();
  t2.join();
  std::cout << "dataRaceTest shared_value:" << shared_value << std::endl;
}

int shared_value2 = 0; // 共享数据
std::mutex shared_value_mutex;
void safe_increament() {
  std::lock_guard<std::mutex> lock(shared_value_mutex);
  for (int i = 0; i < 100000; ++i)
    shared_value2++;
}

void mutexDataRaceTest() {
  std::thread t1(safe_increament);
  std::thread t2(safe_increament);
  t1.join();
  t2.join();
  std::cout << "mutexDataRaceTest shared_value2:" << shared_value2 << std::endl;
}

} // namespace sharingDataBetweenThreads
