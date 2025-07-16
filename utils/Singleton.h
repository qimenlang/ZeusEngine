#include <type_traits>

template <typename T> class Singleton {
public:
  // 获取单例实例
  static T &getInstance() {
    static T instance;
    return instance;
  }

  // 删除拷贝操作
  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;

protected:
  Singleton() = default;
  virtual ~Singleton() = default;
};