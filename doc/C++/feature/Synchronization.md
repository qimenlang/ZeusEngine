线程间同步的高级机制

#### **1. 条件变量（Condition Variables）深度解析**  
- **双组件模型**：  
  1. `std::condition_variable`（或`std::condition_variable_any`）  
  2. 互斥锁（通常`std::mutex`）  
  3. 布尔条件谓词（避免虚假唤醒）

- **标准模式**：  
  ```cpp
  std::mutex mtx;
  std::condition_variable cv;
  bool data_ready = false;  // 条件谓词

  // 生产者
  {
    std::lock_guard<std::mutex> lk(mtx);
    prepare_data();
    data_ready = true;
    cv.notify_one();  // 通知一个等待线程
  }

  // 消费者
  {
    std::unique_lock<std::mutex> lk(mtx);
    cv.wait(lk, []{ return data_ready; }); // 原子性解锁+等待+重锁
    process_data();
  }
  ```

- **关键细节**：  
  - 必须使用`std::unique_lock`（`lock_guard`无法满足解锁需求）  
  - 条件检查必须放在`wait`的谓词中（防御虚假唤醒）  
  - `notify_all()`唤醒所有等待线程（广播模式）

---

#### **2. Future/Promise 异步结果传递**  
##### (1) 核心组件
| 组件          | 作用                          |
|---------------|-------------------------------|
| `std::promise<T>` | 承诺提供未来值（生产者端）   |
| `std::future<T>`  | 获取异步结果（消费者端）     |
| `std::shared_future<T>` | 可多次获取的结果（多消费者）|

##### (2) 典型工作流
```cpp
// 生产者线程
void producer(std::promise<int> result_promise) {
  int value = compute_intensive_task();
  result_promise.set_value(value);  // 传递结果
}

// 消费者线程
int main() {
  std::promise<int> prom;
  std::future<int> fut = prom.get_future();
  
  std::thread t(producer, std::move(prom));
  // ... 其他并行任务 ...
  
  int result = fut.get();  // 阻塞直到结果就绪
  t.join();
}
```

##### (3) 异常传递
```cpp
try {
  dangerous_operation();
} catch(...) {
  prom.set_exception(std::current_exception()); // 捕获并传递异常
}
// 消费者调用 fut.get() 将重新抛出异常
```

---

#### **3. 超时控制（Timeout-Based Synchronization）**  
- **时钟类型**：  
  - `std::chrono::system_clock`（系统时间，可调整）  
  - `std::chrono::steady_clock`（单调时钟，推荐超时控制）

- **超时API整合**：  
  | 同步原语         | 超时方法                      |
  |------------------|-------------------------------|
  | `std::mutex`     | `try_lock_for()`, `try_lock_until()` |
  | `std::condition_variable` | `wait_for()`, `wait_until()` |
  | `std::future`    | `wait_for()`, `wait_until()` |

- **使用范例**：  
  ```cpp
  std::future<int> fut = async_task();
  if(fut.wait_for(500ms) == std::future_status::ready) {
    auto result = fut.get();  // 结果就绪
  } else {
    handle_timeout();         // 超时处理
  }
  ```

---

#### **4. 一次性事件同步（Single-Use Event）**  
- **`std::future`的局限性**：  
  - 只能`get()`一次  
  - 无法重置状态  

- **`std::async`的隐藏陷阱**：  
  ```cpp
  auto fut = std::async(compute_value); 
  // 析构时若未get()会隐式阻塞（策略依赖）
  ```

- **手动控制异步任务**：  
  ```cpp
  std::packaged_task<int()> task([]{ return 42; });
  std::future<int> fut = task.get_future();
  std::thread t(std::move(task));  // 显式线程管理
  t.detach();  // 或join()
  ```

---

#### **5. 复杂同步场景解决方案**  
##### (1) 多线程等待同一事件
```cpp
std::promise<void> start_promise;
std::shared_future<void> start_fut = start_promise.get_future().share();

auto worker = [start_fut] {
  start_fut.wait();  // 所有线程等待同一个future
  do_work();
};

std::vector<std::thread> threads;
for(int i=0; i<5; ++i) 
  threads.emplace_back(worker);

// 触发所有线程
start_promise.set_value(); 
```

##### (2) 截止时间驱动任务
```cpp
auto deadline = std::chrono::steady_clock::now() + 2s;
std::condition_variable_any cv;

bool process_until_deadline() {
  std::unique_lock lk(mtx);
  return cv.wait_until(lk, deadline, []{ 
    return check_condition(); 
  });
}
```

---

### 关键陷阱与最佳实践
| 陷阱类型                  | 后果                     | 规避方案                     |
|---------------------------|--------------------------|------------------------------|
| 条件变量无谓词等待        | 虚假唤醒导致逻辑错误     | 始终使用带谓词的`wait()`     |
| 多次调用`future.get()`    | `std::future_error`异常  | 单次获取或使用`shared_future`|
| `std::async`策略不明确    | 意外串行执行             | 显式指定`std::launch::async` |
| 未处理Promise异常         | 程序终止                 | 生产者端捕获并`set_exception`|
| 共享future拷贝错误        | 数据竞争                 | 值传递或使用`std::ref`包装   |

> 📌 **设计哲学**：  
> - **精确同步**：使用条件变量时，确保通知范围最小化（临界区内通知）  
> - **结果导向**：优先使用future/promise而非共享变量+锁  
> - **超时防御**：所有阻塞操作都应设置超时（除非确实需要无限等待）  
> - **异常安全**：异步任务必须处理异常（未捕获异常导致`std::terminate`）  


1. std::future & std::shared_future
   - std::future
     - 独占性：一个异步结果智能被一个future获取，须通过移动语义转移所有权
     - 一次性消费：get()调用一次即失效
     - 使用场景：单线程等待异步结果，结果只需要消费一次
   - std::shared_future
     - 共享性：可被复制多次，多个对象共享同一异步结果
     - 多次消费：可反复调用get,每次返回结果的**值拷贝**（若结果为引用，需要注意线程安全）
     - 线程安全：各线程持有独立副本时，调用get()不存在条件竞争
2. 