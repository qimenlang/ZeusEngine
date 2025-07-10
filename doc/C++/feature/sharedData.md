多线程数据共享

#### **1. 数据竞争（Data Race）的本质**  
- **定义**：当多个线程同时访问**同一内存位置**，且至少有一个线程执行**写操作**时，且操作**未同步**
- **后果**：未定义行为（程序崩溃、结果错误、安全漏洞）
- **示例**：
  ```cpp
  int shared_value = 0;  // 共享数据
  
  void increment() {
    for(int i=0; i<100000; ++i) 
      ++shared_value;    // 数据竞争！
  }
  
  int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join(); t2.join();
    std::cout << shared_value;  // 可能输出小于200000的值
  }
  ```

---

#### **2. 互斥量（Mutex）的核心解决方案**  
##### (1) `std::mutex` 基础用法
  ```cpp
  std::mutex mtx;
  void safe_increment() {
    std::lock_guard<std::mutex> lock(mtx);  // RAII锁
    ++shared_value;  // 受保护区域
  }  // 自动解锁
  ```
  - **`lock_guard`**：最简单的RAII锁管理器，构造时加锁，析构时解锁

##### (2) 灵活锁管理 `std::unique_lock`
  ```cpp
  std::mutex mtx;
  void transfer() {
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock); // 延迟加锁
    // ... 非临界区操作 ...
    lock.lock();  // 显式加锁
    // ... 临界区 ...
    lock.unlock(); // 可提前解锁
  }
  ```
  - 支持特性：延迟加锁(`defer_lock`)、尝试加锁(`try_lock`)、锁所有权转移

---

#### **3. 死锁（Deadlock）的预防**  
- **典型场景**：  
  ```cpp
  // 线程1              // 线程2
  lock(mutexA);         lock(mutexB);
  lock(mutexB);         lock(mutexA);  // 死锁！
  ```
- **解决方案**：
  1. **固定加锁顺序**：所有线程按相同顺序获取锁
  2. **同时加锁**：使用 `std::lock` 原子性获取多个锁
     ```cpp
     void safe_operation() {
       std::unique_lock<std::mutex> lock1(mutexA, std::defer_lock);
       std::unique_lock<std::mutex> lock2(mutexB, std::defer_lock);
       std::lock(lock1, lock2);  // 原子性锁定
       // 操作共享数据
     }

      void safe_operation() {
      std::scoped_lock lock(mutexA,mutexB); 
       // 操作共享数据
     }
     ```
  3. **避免嵌套锁**：尽量减少锁的作用域

---

#### **4. 接口级数据竞争**  
- **问题代码**：
  ```cpp
  template<typename T>
  class Stack {
    std::stack<T> data;
    mutable std::mutex mtx;
  public:
    T top() const { 
      std::lock_guard<std::mutex> lock(mtx);
      if(data.empty()) throw empty_stack(); 
      return data.top(); 
    }
    void pop() {
      std::lock_guard<std::mutex> lock(mtx);
      data.pop();
    }
  };
  
  // 用户代码：可能在不同线程调用
  if(!s.empty()) {  // 问题点：非原子操作
    T value = s.top();
    s.pop();
  }
  ```
- **解决方案**：合并接口为原子操作
  ```cpp
  std::pair<bool, T> try_pop() {
    std::lock_guard<std::mutex> lock(mtx);
    if(data.empty()) return {false, T()};
    T value = data.top();
    data.pop();
    return {true, value};
  }
  ```

---

#### **5. 保护共享数据的替代方案**  
##### (1) 读写锁 `std::shared_mutex` (C++17)
  ```cpp
  std::shared_mutex rw_mutex;
  void reader() {
    std::shared_lock lock(rw_mutex);  // 共享锁（多读）
    // 读操作
  }
  void writer() {
    std::unique_lock lock(rw_mutex);  // 独占锁（单写）
    // 写操作
  }
  ```

##### (2) 保护初始化 `std::call_once`
  ```cpp
  std::once_flag init_flag;
  void init_resource() { /* 初始化 */ }
  
  void worker() {
    std::call_once(init_flag, init_resource); // 线程安全初始化
    // 使用资源
  }
  ```

##### (3) 递归锁 `std::recursive_mutex`
  ```cpp
  std::recursive_mutex rec_mtx;
  void nested_call(int i) {
    std::lock_guard<std::recursive_mutex> lock(rec_mtx);
    if(i>0) nested_call(i-1);  // 可重入
  }
  ```

---

#### **6. 条件变量（Condition Variables）**  
实现线程间事件通知的同步机制：
```cpp
std::mutex mtx;
std::condition_variable cv;
bool data_ready = false;

// 生产者
void producer() {
  std::lock_guard<std::mutex> lock(mtx);
  prepare_data();
  data_ready = true;
  cv.notify_one();  // 通知消费者
}

// 消费者
void consumer() {
  std::unique_lock<std::mutex> lock(mtx);
  cv.wait(lock, []{ return data_ready; }); // 等待条件
  process_data();
}
```
- **关键点**：
  - 总是配合互斥量使用
  - `wait()` 会自动释放锁并阻塞，被唤醒时重新获取锁
  - 使用谓词防止虚假唤醒（`wait(lock, predicate)`）

---

### 关键陷阱与最佳实践总结
| 问题类型               | 风险                          | 解决方案                     |
|------------------------|-------------------------------|------------------------------|
| 数据竞争               | 未定义行为                    | 互斥量保护所有访问路径       |
| 死锁                   | 程序永久挂起                  | 固定锁顺序/`std::lock`       |
| 接口级竞争             | 数据不一致                    | 设计原子接口                 |
| 虚假唤醒               | 条件变量过早唤醒              | 使用带谓词的`wait()`         |
| 锁粒度过大             | 性能瓶颈                      | 最小化临界区范围             |
| 忘记释放锁             | 死锁                          | 始终使用RAII锁管理器         |
| 锁与数据可见性不匹配   | 部分数据未保护                | 明确锁保护的数据边界         |

> 📌 **核心原则**：共享数据是万恶之源，如必须共享，则所有访问路径必须同步。优先设计无共享架构（如任务并行），其次考虑线程安全容器，最后才使用原始锁机制。