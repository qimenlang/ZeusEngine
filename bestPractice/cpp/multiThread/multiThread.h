#pragma once

namespace multiThreadTest {
struct CallableStruct {
  void operator()() const;
};

void multiThreadBase();
} // namespace multiThreadTest
