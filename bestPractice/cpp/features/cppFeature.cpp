#include <Common.h>

#include <iostream>

#include "FunctionPointer.h"
#include "SmartPointTest.h"
#include "template.h"
#include "variant.h"

int main() {
    PRINT_FUNC_NAME();
    // SmartPointTest::testRawStruct();
    SmartPointTest::testSharedFromThis();
    SmartPointTest::testSharedPtrThreadSafe();
    FunctionPointerTest::test();
    VariantTest::test();

    // 模板测试

    auto num = sum(1, 2, 3, 4, 5);  // C++17 折叠表达式
    std::cout << "Sum: " << num << std::endl;

    print1("Hello, ", "world!", 42, 3.14f, true);  // 输出所有参数
    print2("Hello, ", "world!", 42, 3.14f, true);  // 输出所有参数

    auto left_sub = leftSub(10, 2, 3);    // C++17 折叠表达式
    auto right_sub = rightSub(10, 2, 3);  // C++17 折叠表达式
    std::cout << "Left Sub: " << left_sub << std::endl;
    std::cout << "Right Sub: " << right_sub << std::endl;
}
