#include "SmartPointTest.h"
#include <Common.h>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

namespace SmartPointTest {

void RawStruct::print() { std::cout << "RawStruct print" << std::endl; }

void testRawStruct() {
  PRINT_FUNC_NAME();
  std::shared_ptr<RawStruct> ptr = std::make_shared<RawStruct>();
  std::cout << "raw_ptr: " << ptr.get() << ", use_count: " << ptr.use_count()
            << std::endl;
  ptr->print();
  {
    std::shared_ptr<RawStruct> ptr2 = std::shared_ptr<RawStruct>(ptr.get());
    std::cout << "raw_ptr2: " << ptr2.get()
              << ", use_count: " << ptr2.use_count() << std::endl;
  }
  // ptr2 的析构函数会销毁对象，释放原始指针，导致 ptr 变成野指针,
  std::cout << " ptr use_count: " << ptr.use_count() << std::endl;
  ptr->print();
  // ptr离开作用域时，会第二次销毁同一个对象，导致崩溃
  // 所以不能使用原始指针创建shared_ptr
}

void StructEnabledShared::print() {
  // 使用 std::enable_shared_from_this 允许对象安全的生成指向自身的 shared_ptr
  auto self_ptr = shared_from_this();
  std::cout << "StructEnabledShared print self shadredCount :"
            << self_ptr.use_count() << std::endl;
}

void testSharedFromThis() {
  PRINT_FUNC_NAME();
  // 对象必须是通过 shared_ptr管理
  std::shared_ptr<StructEnabledShared> ptr =
      std::make_shared<StructEnabledShared>();
  std::cout << "StructEnabledShared print shadredCount :" << ptr.use_count()
            << std::endl;
  ptr->print();

  auto ptr2 = ptr->shared_from_this();
  std::cout << "StructEnabledShared print shadredCount :" << ptr2.use_count()
            << std::endl;
  ptr2->print();
  std::cout << "StructEnabledShared print shadredCount :" << ptr2.use_count()
            << std::endl;

  // 不能在栈上创建,抛出 std::bad_weak_ptr
  //   StructEnabledShared instance;
  //   instance.print();
  //  不能再堆上通过new创建
  // StructEnabledShared *raw_ptr = new StructEnabledShared();
  // raw_ptr->print();
}

void testSharedPtrThreadSafe() {
  PRINT_FUNC_NAME();
  struct Data {
    int num;
    std::string name;
  };
  auto shared_data = std::make_shared<Data>();

  auto worker = [shared_data] {
    shared_data->name = "name1";
    std::cout << "name:" << shared_data->name
              << ", use_count:" << shared_data.use_count() << std::endl;
  };

  std::thread t1(worker);
  std::thread t2(worker);
  t1.join();
  t2.join();
}
} // namespace SmartPointTest