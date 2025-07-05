**1. 线程基础操作**  

- **启动线程**  
  ```cpp
  void my_task(); 
  std::thread t(my_task);  // 立即启动线程
  ```
  - 可接受任何**可调用对象**（函数、Lambda、函数对象）
  - 线程在构造时立即启动（无延迟执行）

- **等待线程结束**  
  ```cpp
  t.join();  // 阻塞当前线程，直到t完成
  ```
  - 必须显式调用`join()`或`detach()`，否则析构时`std::terminate`终止程序
  - 调用`join()`后线程对象不再关联实际线程

- **分离线程**  
  ```cpp
  t.detach();  // 让线程在后台自主运行
  ```
  - 分离后无法再`join()`，失去对线程的控制权
  - 典型场景：守护线程（如日志、监控）

---

#### **2. 参数传递陷阱**  
- **默认值传递**  
  ```cpp
  void func(int& param);
  int value = 42;
  std::thread t(func, value);  // 值拷贝传递
  ```
  即使`func`期望引用，线程内部仍使用**参数的副本**。

- **正确传递引用**  
  
  ```cpp
  void modify(int& ref);
  std::thread t(modify, std::ref(value));  // 显式传递引用
  ```
  必须用`std::ref`或`std::cref`包装引用类型。
  
- **传递类成员函数**  
  ```cpp
  class Worker {
    void run();
  };
  Worker w;
  std::thread t(&Worker::run, &w);  // 传递对象指针+成员函数指针
  ```

---

#### **3. 线程所有权转移**  
- **独占性**  
  `std::thread`不可复制（`delete`拷贝构造），仅支持移动语义：  
  
  ```cpp
  std::thread t1(task);
  std::thread t2 = std::move(t1);  // 显式move,所有权转移到t2
  t1 = std::thread(task2);//起新线程临时对象，并且所有权转移到t1
  ```
  
- **容器存储线程**  
  ```cpp
  std::vector<std::thread> workers;
  workers.push_back(std::thread(task1));
  workers.emplace_back(task2);
  for (auto& t : workers) t.join();  // 必须用引用遍历
  ```

---

#### **4. 异常安全（RAII模式）**  
- **问题**：线程启动后，若在`join()`前抛出异常，线程可能泄漏  
  ```cpp
  void unsafe() {
    std::thread t(task);
    throw_exception();  // 若此处异常，t.join()未调用！
    t.join();
  }
  ```

- **解决方案：`thread_guard`**  
  ```cpp
  class thread_guard {
    std::thread& t;
  public:
    explicit thread_guard(std::thread& t_) : t(t_) {}
    ~thread_guard() {
      if (t.joinable()) t.join();  // 析构时确保join
    }
    // 禁用拷贝
    thread_guard(const thread_guard&) = delete;
    thread_guard& operator=(const thread_guard&) = delete;
  };
  ```

---

#### **5. 运行时线程数量决策**  
- **硬件并发数**  
  ```cpp
  unsigned cores = std::thread::hardware_concurrency(); 
  // 返回CPU核心数（可能为0，表示未知）
  ```

- **动态任务划分**  
  ```cpp
  void parallel_process(data_chunk data) {
    const size_t min_per_thread = 25;
    const size_t max_threads = (data.size() + min_per_thread - 1) / min_per_thread;
    const size_t num_threads = std::min(max_threads, std::thread::hardware_concurrency());
    
    std::vector<std::thread> threads;
    for (size_t i = 0; i < num_threads; ++i) {
      threads.emplace_back(process_segment, data.subchunk(i));
    }
    // ...
  }
  ```

---

#### **6. 线程标识符（Thread ID）**  
- **获取方式**  
  ```cpp
  std::thread::id master_id = std::this_thread::get_id();  // 当前线程ID
  std::thread::id worker_id = t.get_id();                 // 线程对象ID
  ```

- **主要用途**  
  
  - 调试日志：标记执行线程
  - 避免死锁：主线程特殊处理（如GUI更新）
  - 关联数据：如线程局部存储(TLS)

---

### 关键陷阱总结
| 陷阱                   | 后果                   | 解决方案             |
| ---------------------- | ---------------------- | -------------------- |
| 未调用`join/detach`    | 析构时`std::terminate` | RAII封装             |
| 传递引用未用`std::ref` | 意外值拷贝             | 显式包装引用         |
| 跨线程访问局部变量     | 悬空引用               | 确保生命周期或值传递 |
| 异常导致`join`跳过     | 线程泄漏               | RAII守卫             |

> 📌 **核心原则**：线程不是特殊资源，必须遵循RAII管理生命周期，所有权转移需显式使用移动语义。