#include "synchronization.h"
#include "ThreadGuard.h"
#include <Common.h>
#include <future>

namespace synchronization {
/*
在线程间传递数据
1.简单异步任务	std::async
2.可调用对象包装	std::packaged_task
3.完全手动控制	std::promise
*/
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

void testasync() {
  auto getResult = []() -> int {
    std::cout << "id:" << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 100;
  };
  // std::async与std::thread 相比：1.不立即启动，根据调度策略可能延迟启动
  // 2.不需要join、detach，自动管理线程生命周期 3.异步任务结果通过future获取
  // 不需要创建共享变量 4.适合短期任务

  // 显示指定调度策略
  // 1.默认策略：async|deferred,自行选择执行方式
  // 2.async:强制创建新线程执行任务,不保证立即执行
  // 3.deferred:延迟到调用get()时运行,不创建新线程，在调用get的线程中运行
  std::cout << "id:" << std::this_thread::get_id() << std::endl;
  std::future<int> result = std::async(getResult);
  // std::future<int> result = std::async(std::launch::async, getResult);
  // std::future<int> result = std::async(std::launch::deferred, getResult);
  // result.wait();
  std::cout << "testasync result get: " << result.get() << std::endl;
  std::cout << "testasync End" << std::endl;
}

void testPromise() {
  std::promise<int> prom;
  // future 与promise建立连接
  std::future<int> fut = prom.get_future();
  std::thread worker([&prom] {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    prom.set_value(11);
  });
  int result = fut.get();
  std::cout << "worker promise result :" << result << std::endl;
  worker.join();
}

void testSharedFuture() {
  std::promise<int> prom;
  std::shared_future<int> fut = prom.get_future().share();

  std::thread sender([&prom]() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    prom.set_value(11);
  });

  ThreadGuard senderGuard(sender);

  std::thread reciver1(std::thread([&fut]() {
    std::cout << "reciver1 recive :" << fut.get() << std::endl;
  }));
  ThreadGuard reciverGuard1(reciver1);

  std::thread reciver2(std::thread([&fut]() {
    std::cout << "reciver2 recive :" << fut.get() << std::endl;
  }));
  ThreadGuard reciverGuard2(reciver2);
}

} // namespace synchronization
