#include "cppFeature.h"
#include "FunctionPointer.h"
#include "variant.h"

namespace cppFeature {
void run() {
  PRINT_FUNC_NAME();
  FunctionPointerTest::test();
  VariantTest::test();
}
} // namespace cppFeature