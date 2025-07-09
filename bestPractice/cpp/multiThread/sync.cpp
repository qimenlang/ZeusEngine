#include "sync.h"
#include <Common.h>

namespace sync {
std::queue<int> dataQueue;
std::mutex mutex;
std::condition_variable cond;
int value;

void product() {
  while (value < 100) {
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      std::lock_guard<std::mutex> lock(mutex);
      std::cout << std::this_thread::get_id() << ",product : " << value
                << "size:" << dataQueue.size() << std::endl;
      dataQueue.push(value++);
    }
    // 唤醒一个正在等待该条件变量的线程（具体唤醒哪个线程由系统调度决定）
    cond.notify_one();
    // std::cout << "product notify" << std::endl;
  }
}

void consume() {
  while (value < 100) {
    // std::cout << "Consume" << std::endl;
    // unique_lock可以灵活加锁解锁
    std::unique_lock<std::mutex> lock(mutex);
    // wait在条件满足时，直接返回，继续运行；条件不满足时，解锁互斥，阻塞线程；
    // 线程阻塞后又被唤醒时，wait重新给互斥加锁(所以必须用unique_lock)；
    cond.wait(lock, [] { return dataQueue.size(); });
    auto data = dataQueue.front();
    dataQueue.pop();
    lock.unlock();
    std::cout << std::this_thread::get_id() << ",Consume:" << data
              << "size:" << dataQueue.size() << std::endl;
  }
}

void testConditionVariable() {
  PRINT_FUNC_NAME();
  std::thread productor(product);
  std::thread consumer(consume);
  productor.join();
  consumer.join();
  std::cout << "testConditionVariable END" << std::endl;
}

void product2(threadSafeQueue<int> &queue) {
  while (value < 100) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    queue.push(value++);
    std::cout << std::this_thread::get_id() << ",product : " << value
              << "size:" << dataQueue.size() << std::endl;
  }
}

void consume2(threadSafeQueue<int> &queue) {
  while (value < 100) {
    int data;
    queue.waitAndPop(data);
    std::cout << std::this_thread::get_id() << ",Consume:" << data
              << "size:" << dataQueue.size() << std::endl;
  }
}

void consume3(threadSafeQueue<int> &queue) {
  while (value < 100) {
    auto dataPtr = queue.waitAndPop();
    std::cout << std::this_thread::get_id() << ",Consume:" << *dataPtr
              << "size:" << dataQueue.size() << std::endl;
  }
}

void testConditionVariable2() {
  PRINT_FUNC_NAME();

  threadSafeQueue<int> queue;
  std::thread t1(product2, std::ref(queue));
  std::thread t2(consume2, std::ref(queue));
  std::thread t3(consume3, std::ref(queue));

  t1.join();
  t2.join();
  t3.join();
  std::cout << " testConditionVariable2 END" << std::endl;
}

} // namespace sync
