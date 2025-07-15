#include <core/event.h>

#include <catch2/catch_test_macros.hpp>

void test(int value) {}
TEST_CASE("test event template support lambda", "[Event]") {
    Event<void(int)> event;
    auto func = [](int value) {};

    event.add(test);
    event.add(func);
    REQUIRE(event.m_functions.size() == 2);
}
