#include "FunctionPointer.h"
#include "SmartPointTest.h"
#include "variant.h"
#include <Common.h>
#include <iostream>

int main() {
  PRINT_FUNC_NAME();
  // SmartPointTest::testRawStruct();
  SmartPointTest::testSharedFromThis();
  SmartPointTest::testSharedPtrThreadSafe();
  FunctionPointerTest::test();
  VariantTest::test();
}
