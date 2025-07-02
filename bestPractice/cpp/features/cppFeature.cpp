#include "cppFeature.h"
#include "FunctionPointer.h"
#include "SmartPointTest.h"
#include "variant.h"

namespace cppFeature {
void run() {
  PRINT_FUNC_NAME();
  // SmartPointTest::testRawStruct();
  SmartPointTest::testSharedFromThis();
  SmartPointTest::testSharedPtrThreadSafe();
  FunctionPointerTest::test();
  VariantTest::test();
}
} // namespace cppFeature