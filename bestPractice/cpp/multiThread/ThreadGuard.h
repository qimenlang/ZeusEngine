#pragma once
#include <iostream>
#include <thread>

// thread guard
struct ThreadGuard {
    ThreadGuard(std::thread &t) : m_thread(t) {}
    ~ThreadGuard() {
        if (m_thread.joinable()) {
            m_thread.join();
            std::cout << "Origin thread joined" << std::endl;
        }
    }
    ThreadGuard(const ThreadGuard &) = delete;
    ThreadGuard &operator=(const ThreadGuard &) = delete;
    std::thread &m_thread;
};