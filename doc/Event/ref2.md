### 事件驱动开发最佳实践（C++ 实现指南）

事件驱动开发的核心是**异步处理**和**解耦**，以下是关键实践及 C++ 示例：

---

#### 1. **事件循环 (Event Loop) - 核心引擎**
```cpp
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>

class EventLoop {
public:
    using EventHandler = std::function<void()>;

    void postEvent(EventHandler handler) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(std::move(handler));
        cv.notify_one();
    }

    void run() {
        while (running) {
            EventHandler handler;
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [&] { return !queue.empty() || !running; });
                
                if (!running) break;
                
                handler = std::move(queue.front());
                queue.pop();
            }
            handler(); // 执行事件处理
        }
    }

    void stop() {
        running = false;
        cv.notify_all();
    }

private:
    std::queue<EventHandler> queue;
    std::mutex mtx;
    std::condition_variable cv;
    bool running = true;
};
```

---

#### 2. **事件对象封装 - 避免原始数据**
```cpp
struct Event {
    enum Type { MOUSE_CLICK, KEY_PRESS, NETWORK_DATA };
    Type type;
    std::string data; // 结构化数据（如JSON）
    
    // 事件携带上下文
    uint64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();
};
```

---

#### 3. **发布-订阅模式 - 解耦组件**
```cpp
#include <map>
#include <vector>

class EventBus {
public:
    using Listener = std::function<void(const Event&)>;

    void subscribe(Event::Type type, Listener listener) {
        listeners[type].push_back(std::move(listener));
    }

    void publish(const Event& event) {
        auto it = listeners.find(event.type);
        if (it != listeners.end()) {
            for (auto& listener : it->second) {
                listener(event); // 通知所有订阅者
            }
        }
    }

private:
    std::map<Event::Type, std::vector<Listener>> listeners;
};
```

---

#### 4. **异步 I/O - 使用 Boost.Asio**
```cpp
#include <boost/asio.hpp>

void asyncNetworkExample() {
    boost::asio::io_context io;
    boost::asio::ip::tcp::socket socket(io);
    
    // 异步连接
    socket.async_connect(
        boost::asio::ip::tcp::endpoint(
            boost::asio::ip::make_address("127.0.0.1"), 8080
        ),
        [](const boost::system::error_code& ec) {
            if (!ec) std::cout << "Connected!\n";
        }
    );
    
    io.run(); // 启动事件循环
}
```

---

#### 5. **状态管理 - 避免阻塞**
```cpp
// 使用状态机处理复杂逻辑
enum class State { IDLE, CONNECTING, PROCESSING };

class Connection {
public:
    void onDataReceived(const Event& event) {
        switch (state) {
            case State::IDLE:
                startProcessing(event.data);
                state = State::PROCESSING;
                break;
            case State::PROCESSING:
                queueEvent(event); // 非阻塞排队
                break;
        }
    }

private:
    State state = State::IDLE;
    std::queue<Event> pendingEvents;
};
```

---

#### 6. **资源管理 - 防止泄漏**
```cpp
// 使用智能指针管理事件处理器
auto handler = std::make_shared<EventHandler>([]{
    std::cout << "Event processed\n";
});

eventLoop.postEvent([handler]{
    (*handler)(); // 共享所有权确保安全
});
```

---

#### 7. **性能优化 - 批量处理**
```cpp
// 合并高频事件（如鼠标移动）
void coalesceEvents() {
    static std::optional<Event> lastMouseEvent;
    
    auto submitEvent = [](const Event& e) {
        if (!lastMouseEvent || e.timestamp > lastMouseEvent->timestamp + 10) {
            eventBus.publish(e);
        }
        lastMouseEvent = e;
    };
}
```

---

### 关键最佳实践总结：
1. **单线程事件循环**：避免锁竞争（如 GUI 主线程）
2. **事件轻量化**：传递数据指针而非拷贝大数据
3. **超时处理**：添加事件超时检测防止饥饿
   ```cpp
   cv.wait_for(lock, 100ms, [&]{ return !queue.empty(); });
   ```
4. **优先级队列**：紧急事件优先处理
   ```cpp
   std::priority_queue<Event, std::vector<Event>, ComparePriority> queue;
   ```
5. **调试支持**：
   - 事件日志追踪
   - 序列化事件用于回放

> **推荐库**：  
> - 网络：Boost.Asio / libuv  
> - 信号槽：Qt Signals/Slots, Boost.Signals2  
> - 消息队列：ZeroMQ, RabbitMQ C++ client