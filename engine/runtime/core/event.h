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
    using HandleType = uint32_t;

    HandleType add(FuncType&& fn) {
        ++m_handle;
        m_functions.emplace_back(std::move(fn));
        return m_handle;
    }

    HandleType add(FuncType& fn) {
        ++m_handle;
        m_functions.emplace_back(fn);
        return m_handle;
    }

    void remove(HandleType handle) {
        if (handle > 0 && handle <= m_functions.size()) {
            m_functions.erase(m_functions.begin() + (handle - 1));
        }
    }

    void invoke(Args... args) {
        for (const auto& func : m_functions) {
            func(args...);
        }
    }

    operator bool() const noexcept { return !m_functions.empty(); }

   private:
    std::vector<FuncType> m_functions;
    HandleType m_handle = 0;
};
