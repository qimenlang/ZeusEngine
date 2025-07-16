#include <Common.h>

#include <iostream>

#include "FunctionPointer.h"
#include "SmartPointTest.h"
#include "variant.h"

int main() {
    PRINT_FUNC_NAME();
    // SmartPointTest::testRawStruct();
    SmartPointTest::testSharedFromThis();
    SmartPointTest::testSharedPtrThreadSafe();
    FunctionPointerTest::test();
    VariantTest::test();
}
