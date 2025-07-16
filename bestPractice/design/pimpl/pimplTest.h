#pragma once
#include <memory>
namespace design {
class pimplTest {
   public:
    pimplTest(/* args */);
    ~pimplTest();
    void process();

   private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};
}  // namespace design
