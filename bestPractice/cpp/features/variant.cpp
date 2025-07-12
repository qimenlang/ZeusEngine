#include "variant.h"

#include <Common.h>

#include <iostream>

namespace VariantTest {

void test() {
    PRINT_FUNC_NAME();
    // Assigning an integer to the variant
    // 将最常用/最小类型放前面（减少内存占用）：
    std::variant<int, double, std::string> testData;
    // Accessing the value as an integer
    // variant 默认初始化选择第一个类型，index 返回该类型的索引 0
    if (std::holds_alternative<int>(testData)) {
        int value = std::get<int>(testData);
        std::cout << "Integer value: " << value
                  << ",index: " << testData.index() << std::endl;
    }

    testData = 42;

    // Accessing the value as an integer
    if (std::holds_alternative<int>(testData)) {
        int value = std::get<int>(testData);
        std::cout << "Integer value: " << value
                  << ",index: " << testData.index() << std::endl;
    }

    // Assigning a double to the variant
    testData = 3.14;

    // Accessing the value as a double
    if (std::holds_alternative<double>(testData)) {
        double value = std::get<double>(testData);
        std::cout << "Double value: " << value << ",index: " << testData.index()
                  << std::endl;
    }

    // Assigning a string to the variant
    testData = std::string("Hello, Variant!");

    // Accessing the value as a string
    if (std::holds_alternative<std::string>(testData)) {
        std::string value = std::get<std::string>(testData);
        std::cout << "String value: " << value << "index: " << testData.index()
                  << std::endl;
    }

    // 使用 std::visit 访问variant,可以保证类型安全
    std::visit(
        [](const auto &value) {
            std::cout << "Visiting value: " << value << std::endl;
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, int>) {
                std::cout << "It's an int!" << std::endl;
            } else if constexpr (std::is_same_v<T, double>) {
                std::cout << "It's a double!" << std::endl;
            } else if constexpr (std::is_same_v<T, std::string>) {
                std::cout << "It's a string!" << std::endl;
            }
        },
        testData);

    // 使用get_if进行类型安全检查
    if (auto *p = std::get_if<std::string>(&testData)) {
        std::cout << "Pointer to string value: " << *p << std::endl;
    }
}
}  // namespace VariantTest
