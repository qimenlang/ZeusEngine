#pragma once
#include <memory>
class Widget {
public:
  Widget(/* args */);
  ~Widget();
  void process();

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;
};
