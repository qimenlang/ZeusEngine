#include <Engine.h>

#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <cstdint>
#include <string>

TEST_CASE("Load Standford Dragon", "[Model Load]") {
    std::string dragonPath =
        std::string(ZEUS_ROOT_DIR).append("/model/Stanford/dragon.obj");
    auto now = std::chrono::high_resolution_clock::now();
    Zeus::Engine::getInstance().assetManager().loadModel(dragonPath);
    auto timeSpent = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::high_resolution_clock::now() - now)
                         .count();
    std::cout << "Load Standford Dragon time: " << timeSpent << " ms"
              << std::endl;
}