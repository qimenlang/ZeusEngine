#pragma once

#include <functional>
#include <vector>
/**
 * @brief 事件回调
 *
 * @tparam T
 */
template <typename T>
class Event;

/**
 * @brief 事件回调
 *
 * @tparam Ret 回调返回值
 * @tparam Args 回调参数
 */
template <typename Ret, typename... Args>
class Event<Ret(Args...)> {
   public:
    using RawType = Ret(Args...);
    using FuncType = std::function<RawType>;
    void add(FuncType&& fn) { m_functions.emplace_back(std::move(fn)); }
    void invoke(Args... args) {
        for (const auto& func : m_functions) {
            func(args...);
        }
    }

    std::vector<FuncType> m_functions;

   private:
};
