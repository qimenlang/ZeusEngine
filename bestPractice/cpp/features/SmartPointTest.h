#pragma once
#include <memory>

namespace SmartPointTest {

struct RawStruct {
    void print();
};

void testRawStruct();
// SharedFromThis
// std::enable_shared_from_this 是模板基类，允许对象安全的生成指向自身的
// shared_ptr，而不会创建新的控制块
// 作用：
//       1.安全获取自身shader_ptr:允许对象在其成员函数中安全地创建指向自身的shared_ptr
//       2.避免多控制块问题： 避免多个独立的 shared_ptr 控制同一对象，
//       一个shared_ptr 管理了对象，使用其原始指针创建了另一个shader_ptr,
//       两个shader_ptr是独立的引用计数，一个释放了，
//       另一个变成野指针，对象也会被多次析构造成未定义

struct StructEnabledShared
    : public std::enable_shared_from_this<StructEnabledShared> {
    void print();
};

void testSharedFromThis();

// SharedPtrThreadSafe
void testSharedPtrThreadSafe();
}  // namespace SmartPointTest
