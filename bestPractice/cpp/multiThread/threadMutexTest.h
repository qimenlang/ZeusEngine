#include <exception>
#include <memory>
#include <mutex>
#include <stack>

namespace threadMutexTest {
struct emptyStackException : std::exception {
  // 重载what()方法并提供异常消息
  // c++11 noexcept = c++98 throw()
  const char *what() const noexcept override { return "emptyStack"; }
};

struct emptyStackRunError : std::runtime_error {
  emptyStackRunError() : std::runtime_error("emptyStack") {}
  /* data */
};

template <typename T> class threadsafeStack {
private:
  std::stack<T> m_stack;
  mutable std::mutex m_mutex;

public:
  threadsafeStack(/* args */) {};
  ~threadsafeStack() {};
  threadsafeStack(const threadsafeStack &other) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_stack = other.m_stack;
  }

  threadsafeStack &operator=(const threadsafeStack &other) = delete;
  void push(T t) {
    // m_mutex.lock();
    // std::lock_guard: RAII
    std::lock_guard<std::mutex> lock(m_mutex);
    m_stack.push(t);
    // m_mutex.unlock();
  }
  void pop(T &t) {
    // std::lock_guard: 异常安全
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_stack.empty())
      throw emptyStackException();
    t = m_stack.top();
    m_stack.pop();
  }

  std::shared_ptr<T> pop() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_stack.empty())
      throw emptyStackRunError();
    std::shared_ptr<T> res(std::make_shared<T>(m_stack.top()));
    m_stack.pop();
    return res;
  }

  bool empty() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_stack.empty();
  }
  size_t size() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_stack.size();
  }
};

void threadsafeStackTest();

} // namespace threadMutexTest