### C++ 多线程编程：从入门到深入

#### 一、基础概念
1. **线程 vs 进程**
   - 线程：轻量级执行单元，共享进程内存空间
   - 进程：独立内存空间，创建销毁开销大

2. **并发模型**
   - 并行（**Parallelism / Parallel**）：多核同时执行、同时执行多个任务
   - 并发：(**Concurrency / Concurrent**) :  单核时间片轮转、交替处理多个任务

#### 二、基础使用（C++11起）
```cpp
#include <iostream>
#include <thread>

void hello() {
    std::cout << "Hello from thread " 
              << std::this_thread::get_id() << "\n";
}

int main() {
    std::thread t1(hello);  // 创建线程
    std::thread t2([]{
        std::cout << "Lambda thread\n";
    });
    
    t1.join();  // 等待结束
    t2.join();
    
    // t.detach(); // 分离线程（慎用）
}
```

#### 三、数据共享与同步
##### 1. 互斥锁（mutex）：为了防止多个线程同时访问共享资源
```cpp
#include <mutex>

std::mutex mtx;
int shared_data = 0;

void safe_increment() {
    std::lock_guard<std::mutex> lock(mtx);  // RAII锁
    // 或 unique_lock（更灵活）
    ++shared_data;
}
```

##### 2. 条件变量（Condition Variables）
```cpp
std::condition_variable cv;
bool data_ready = false;

// 生产者
{
    std::lock_guard<std::mutex> lk(mtx);
    data_ready = true;
    cv.notify_one();  // 通知消费者
}

// 消费者
{
    std::unique_lock<std::mutex> lk(mtx);
    cv.wait(lk, []{ return data_ready; });  // 等待条件
}
```

##### 3. 原子操作（Atomic）
```cpp
#include <atomic>
std::atomic<int> counter(0);

void atomic_inc() {
    counter.fetch_add(1, std::memory_order_relaxed);
}
```

#### 四、高级特性
##### 1. Future/Promise（异步结果）
```cpp
#include <future>

int compute() { /* 耗时计算 */ return 42; }

int main() {
    std::future<int> fut = std::async(compute);
    std::cout << "Result: " << fut.get();  // 阻塞获取结果
}
```

##### 2. 线程池（C++17+）
```cpp
#include <execution>
std::vector<int> data(1000);

// 自动并行化
std::for_each(std::execution::par, data.begin(), data.end(), 
    [](auto& x){ x = process(x); });
```

##### 3. 屏障（Barrier - C++20）
```cpp
#include <barrier>
std::barrier sync_point(4);  // 等待4个线程

void worker() {
    do_work();
    sync_point.arrive_and_wait();  // 同步点
}
```

#### 五、最佳实践
1. **锁的使用原则**
   - 使用RAII包装器（`lock_guard`/`unique_lock`）
   - 避免嵌套锁
   - 锁粒度尽可能小

2. **线程安全设计**
   ```cpp
   // 线程安全队列示例
   template<typename T>
   class SafeQueue {
       std::queue<T> q;
       mutable std::mutex mtx;
       std::condition_variable cv;
   public:
       void push(T value) {
           std::lock_guard lk(mtx);
           q.push(std::move(value));
           cv.notify_one();
       }
       T pop() {
           std::unique_lock lk(mtx);
           cv.wait(lk, [&]{ return !q.empty(); });
           T val = std::move(q.front());
           q.pop();
           return val;
       }
   };
   ```

3. **性能优化**
   - 优先使用`atomic`代替锁
   - 避免虚假共享（False Sharing）：
     ```cpp
     struct alignas(64) CacheLineAligned {  // 64字节对齐
         int data;
     };
     ```
   - 任务窃取（Work-Stealing）线程池

4. **错误处理**
   - 捕获线程内异常：
     ```cpp
     std::promise<int> p;
     auto fut = p.get_future();
     std::thread([&p]{
         try { /* 可能抛异常 */ }
         catch(...) { p.set_exception(std::current_exception()); }
     }).detach();
     
     try { fut.get(); }
     catch(...) { /* 处理异常 */ }
     ```

#### 六、适用场景
| **场景**      | **推荐技术**               | **示例**          |
| ------------- | -------------------------- | ----------------- |
| I/O密集型任务 | 线程池+异步I/O             | 网络服务器        |
| 数据并行      | `std::for_each(par_unseq)` | 图像处理/矩阵运算 |
| 生产者-消费者 | 阻塞队列+条件变量          | 日志系统          |
| 状态共享      | 原子操作+无锁数据结构      | 实时计数器        |
| 复杂任务依赖  | `std::future`链            | 多步骤计算流程    |
| 实时系统      | 优先级队列+定时器          | 工业控制系统      |

#### 七、避坑指南
1. **死锁预防**
   - 固定锁获取顺序
   - 使用`std::scoped_lock`（C++17）多锁
     ```cpp
     std::mutex mtx1, mtx2;
     std::scoped_lock lk(mtx1, mtx2);  // 原子获取多个锁
     ```

2. **生命周期管理**
   - 确保线程访问的对象生命周期足够长
   - 避免在析构函数中join线程

3. **性能陷阱**
   - 线程创建开销（约10μs/线程）
   - 锁竞争导致的吞吐量下降
   - 缓存一致性协议（MESI）开销

#### 八、现代C++特性（C++17/20）
1. `std::jthread`（C++20）
   - 自动join
   - 支持协作取消
   ```cpp
   std::stop_token st;
   std::jthread worker([&](std::stop_token token) {
       while (!token.stop_requested()) {
           // 工作...
       }
   });
   // 自动join
   ```

2. `std::atomic_ref`（C++20）
   ```cpp
   int non_atomic_var = 0;
   void thread_func() {
       std::atomic_ref ref(non_atomic_var);
       ref.fetch_add(1);
   }
   ```

3. `std::latch`/`std::barrier`（C++20）
   - 轻量级线程同步原语

#### 九、调试工具
1. **TSan（ThreadSanitizer）**
   ```bash
   clang++ -fsanitize=thread -g program.cpp
   ```
2. **Lockfree 调试**
   - `valgrind --tool=helgrind`
   - `std::atomic` 的 `is_lock_free()` 方法

#### 十、总结
| **技术选择**  | **适用场景**                   |
| ------------- | ------------------------------ |
| `std::thread` | 简单任务，明确生命周期管理     |
| 线程池        | 高并发任务，避免创建销毁开销   |
| 原子操作      | 简单共享状态（计数器、标志位） |
| `std::async`  | 异步结果获取，简单并行         |
| 无锁数据结构  | 高性能场景（需专业实现）       |

**黄金法则**：
- 优先使用任务并行（`std::async`）而非线程管理
- 共享数据最小化
- 用消息传递（如队列）代替共享状态
- 性能关键部分使用无锁编程（但需充分测试）

> 实际案例：使用线程池处理HTTP请求
> ```cpp
> ThreadPool pool(4);  // 4个线程
> 
> while (auto req = get_http_request()) {
>  pool.enqueue([req] {
>      auto resp = process_request(req);
>      send_response(resp);
>  });
> }
> ```