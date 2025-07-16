#pragma once
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

namespace synchronization {

template <typename T>
class threadSafeQueue {
   private:
    // 互斥量必须声明为 mutable,以允许在const成员函数中加锁解锁
    mutable std::mutex m_mutex;
    std::queue<T> m_queue;
    std::condition_variable m_cond;

   public:
    threadSafeQueue(/* args */) {};
    threadSafeQueue(const threadSafeQueue &other) {
        std::lock_guard<std::mutex> lc(other.m_mutex);
        m_queue = other.m_queue;
    };
    threadSafeQueue &operator=(const threadSafeQueue &other) = delete;
    ~threadSafeQueue() {};

    void push(const T &t) {
        std::lock_guard<std::mutex> lc(m_mutex);
        m_queue.push(t);
        m_cond.notify_one();
    }
    void tryPop(T &t) {
        std::lock_guard<std::mutex> lc(m_mutex);
        if (m_queue.empty()) return;
        t = m_queue.front();
        m_queue.pop();
    }

    std::shared_ptr<T> tryPop() {
        std::lock_guard<std::mutex> lc(m_mutex);
        if (m_queue.empty()) return std::shared_ptr<T>();
        std::shared_ptr<T> res = std::make_shared<T>(m_queue.front());
        m_queue.pop();
        return res;
    }
    void waitAndPop(T &t) {
        std::unique_lock<std::mutex> lc(m_mutex);
        m_cond.wait(lc, [this] { return !m_queue.empty(); });
        t = m_queue.front();
        m_queue.pop();
    }
    std::shared_ptr<T> waitAndPop() {
        std::unique_lock<std::mutex> lc(m_mutex);
        m_cond.wait(lc, [this] { return !m_queue.empty(); });
        std::shared_ptr<T> res = std::make_shared<T>(m_queue.front());
        m_queue.pop();
        return res;
    }
};

void testConditionVariable();
void testConditionVariable2();
void testasync();
void testPromise();
void testSharedFuture();

}  // namespace synchronization
