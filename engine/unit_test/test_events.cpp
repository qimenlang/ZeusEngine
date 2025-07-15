#include <core/event.h>

#include <catch2/catch_test_macros.hpp>

void test(int value){};
TEST_CASE("test event template support function pointer", "[Event]") {
    Event<void(int)> event;
    event.add(test);
    REQUIRE(bool(event));
}

TEST_CASE("test event template support lambda", "[Event]") {
    Event<void(int)> event;
    auto func = [](int value) {};
    event.add(func);
    REQUIRE(bool(event));
}

TEST_CASE("test event template add and remove", "[Event]") {
    Event<void(int)> event;
    auto func = [](int value) {};
    auto handle = event.add(func);
    REQUIRE(bool(event));
    event.remove(handle);
    REQUIRE(!bool(event));
}

TEST_CASE("test event template invoke when add and remove", "[Event]") {
    bool testInvoked = false;
    auto func = [&](int value) { testInvoked = true; };

    SECTION("event add and invoke") {
        Event<void(int)> event;
        event.add(func);
        event.invoke(42);
        REQUIRE(testInvoked);
    }

    SECTION("event remove and not invoke") {
        Event<void(int)> event;
        auto handle = event.add(func);
        event.remove(handle);
        event.invoke(42);
        REQUIRE(testInvoked == false);
    }
}
