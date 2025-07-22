#include <function/framework/component/mesh_component.h>
#include <function/framework/component/transform_component.h>
#include <function/framework/object/object.h>

#include <catch2/catch_test_macros.hpp>
#include <cstdint>

TEST_CASE("has default transform component", "[Object]") {
    Object object;
    REQUIRE(object.transform() != nullptr);
    REQUIRE(object.getAllComponents().size() == 1);
}

TEST_CASE("add & remove components", "[Object]") {
    Object object;
    SECTION("add components") {
        std::unique_ptr<MeshComponent> component =
            std::make_unique<MeshComponent>();
        object.addComponent(std::move(component));
        REQUIRE(object.getAllComponents().size() == 2);
    }
    SECTION("remove components") {
        std::unique_ptr<MeshComponent> component =
            std::make_unique<MeshComponent>();

        object.addComponent(std::move(component));
        REQUIRE(object.getAllComponents().size() == 2);

        auto allComponents = object.getAllComponents();
        for (auto component : allComponents) {
            object.removeComponent(component);
        }
        REQUIRE(object.getAllComponents().size() == 0);
    }
    SECTION("remove transform components") {
        auto transform = object.transform();
        object.removeComponent(transform);
        REQUIRE(object.getAllComponents().size() == 0);
    }
}
TEST_CASE("get components", "[Object]") {
    Object object;
    REQUIRE(object.transform() == object.getComponent<TransformComponent>());

    std::unique_ptr<MeshComponent> meshComponent =
        std::make_unique<MeshComponent>();
    auto meshRaw = meshComponent.get();
    object.addComponent(std::move(meshComponent));
    REQUIRE(meshRaw == object.getComponent<MeshComponent>());
}