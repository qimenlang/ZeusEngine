#include "pimplTest.h"

#include <iostream>
#include <string>
#include <vector>
namespace design {
// 实现内部类的完整定义
struct pimplTest::Impl {
    std::string name;
    std::vector<int> data;
    void internalProcess() {
        std::cout << "pimplTest::Impl::internalProcess()" << std::endl;
    }
};

pimplTest::pimplTest() : pimpl(std::make_unique<Impl>()) {}

pimplTest::~pimplTest() = default;  // 在Impl完整定义之后声明默认析构

void pimplTest::process() {
    pimpl->internalProcess();  // 委托给pimpl实现
}
}  // namespace design
