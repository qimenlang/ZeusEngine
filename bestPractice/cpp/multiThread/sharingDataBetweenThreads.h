#include <exception>
#include <memory>
#include <mutex>
#include <stack>

namespace sharingDataBetweenThreads {
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

  friend void swapWithDeadLock(threadsafeStack &left, threadsafeStack &right) {
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

  friend void swapWithoutDeakLock(threadsafeStack &left,
                                  threadsafeStack &right) {
    if (&left == &right)
      return;
    // 同时锁住两个互斥，防止死锁
    std::lock(left.m_mutex, right.m_mutex);
    // 互斥的所有权转移到lock_guard, adopt_lock表示mutex已被锁住；
    std::lock_guard lock_left(left.m_mutex, std::adopt_lock);
    std::lock_guard lock_right(right.m_mutex, std::adopt_lock);
    left.m_stack.swap(right.m_stack);
  }

  friend void swapWithScopeLock(threadsafeStack &left, threadsafeStack &right) {
    if (&left == &right)
      return;
    std::scoped_lock(left.m_mutex, right.m_mutex);
    left.m_stack.swap(right.m_stack);
  }
  friend void swapWithUniqueLock(threadsafeStack &left,
                                 threadsafeStack &right) {
    if (&left == &right)
      return;
    // unique_lock : RAII ,获取互斥的控制权，defer_lock : 延迟锁定
    std::unique_lock<std::mutex> lock_left(left.m_mutex, std::defer_lock);
    std::unique_lock<std::mutex> lock_right(right.m_mutex, std::defer_lock);
    std::lock(lock_left, lock_right);
    left.m_stack.swap(right.m_stack);
  }
};

void threadsafeStackTest();
void threadsafeStackSwapNoDeadLockTest();

void dataRaceTest();

void mutexDataRaceTest();

} // namespace sharingDataBetweenThreads