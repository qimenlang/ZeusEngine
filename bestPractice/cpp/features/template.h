#pragma once
#include <iostream>

// 可变参
template <typename... Args>
auto sum(Args... args) {
    return (args + ...);  // C++17 折叠表达式
}

template <typename... Args>
void print1(Args&&... args) {
    (std::cout << ... << args);  // 输出所有参数
    std::cout << std::endl;
}

template <typename... Args>
void print2(Args&&... args) {
    ((std::cout << args << " "), ...);  // 输出所有参数
    std::cout << std::endl;
}

//// C++17 折叠表达式
// 左折叠
template <typename... Args>
auto leftSub(Args... args) {
    return (... - args);  // C++17 折叠表达式
}

// 右折叠
template <typename... Args>
auto rightSub(Args... args) {
    return (args - ...);
}
