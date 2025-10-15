#include "InstancingStarScene.h"

#include <array>

#include "function/render/Material.h"

void InstancingStarScene::init() {
    glm::vec3 planet_pos = {0, -20, -70};
    std::string planet_vs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/default.vs");
    std::string planet_fs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/sampleDiffuse.fs");
    auto planet_mat = Material::create(planet_vs_path, planet_fs_path);

    auto planet_model_path =
        std::string(ZEUS_ROOT_DIR).append("/model/planet/planet.obj");
    auto planet =
        std::make_unique<Object>(planet_model_path.c_str(), planet_mat);
    auto transform = planet->getComponent<TransformComponent>();
    transform->setPosition(planet_pos);
    transform->setScale(glm::vec3{5.f});
    m_objects.emplace_back(std::move(planet));

    const int amount = 10000;

    std::array<glm::mat4, amount> modelMatrices;
    // srand();  // initialize random seed
    float radius = 50.0;
    float offset = 10.0f;
    for (unsigned int i = 0; i < amount; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range
        // [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement =
            (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f;  // keep height of asteroid field smaller
                                        // compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, planet_pos + glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked
        // rotation axis vector
        float rotAngle = static_cast<float>((rand() % 360));
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }

    // 配置instance model mat buffer
    unsigned int instance_mat_bo;
    glGenBuffers(1, &instance_mat_bo);
    glBindBuffer(GL_ARRAY_BUFFER, instance_mat_bo);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0],
                 GL_STATIC_DRAW);

    std::string rock_vs_path =
        std::string(ZEUS_ROOT_DIR)
            .append("/shader/instancing/instancing_mat.vs");
    std::string rock_fs_path =
        std::string(ZEUS_ROOT_DIR).append("/shader/sampleDiffuse.fs");
    auto rock_mat = Material::create(rock_vs_path, rock_fs_path);

    auto rock_model_path =
        std::string(ZEUS_ROOT_DIR).append("/model/rock/rock.obj");
    auto geometrys =
        Zeus::Engine::getInstance().assetManager().loadModel(rock_model_path);
    PrimitiveList primitives;

    bool test_instance = true;
    for (const auto& geometry : geometrys) {
        Primitive primitive{geometry, rock_mat->defaultInstance()->duplicate()};
        primitive.instancing = test_instance;
        primitive.instance_count = amount;
        primitives.emplace_back(primitive);
    }

    if (test_instance) {
        // MeshComponent attrib 已经使用了7个
        for (auto primitive : primitives) {
            glBindVertexArray(primitive.VAO);
            glBindBuffer(GL_ARRAY_BUFFER, instance_mat_bo);
            glEnableVertexAttribArray(7);
            glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                                  (void*)0);
            glEnableVertexAttribArray(8);
            glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                                  (void*)(sizeof(glm::vec4)));
            glEnableVertexAttribArray(9);
            glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                                  (void*)(2 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(10);
            glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                                  (void*)(3 * sizeof(glm::vec4)));
            glVertexAttribDivisor(7, 1);
            glVertexAttribDivisor(8, 1);
            glVertexAttribDivisor(9, 1);
            glVertexAttribDivisor(10, 1);
            glBindVertexArray(0);
        }
        auto rock = std::make_unique<Object>(rock_mat);
        rock->addComponent<MeshComponent>(primitives);
        auto rock_transform = rock->getComponent<TransformComponent>();
        auto pos = glm::vec3{0, 0, -10};
        rock_transform->setPosition(pos);
        m_objects.emplace_back(std::move(rock));
    } else {
        for (int i = 0; i < amount; i++) {
            auto rock = std::make_unique<Object>(planet_mat);
            rock->addComponent<MeshComponent>(primitives);
            auto rock_transform = rock->getComponent<TransformComponent>();
            auto pos = glm::vec3{rand() % 100, rand() % 100, -rand() % 100};
            rock_transform->setPosition(pos);
            rock_transform->setScale(glm::vec3{2.f});
            m_objects.emplace_back(std::move(rock));
        }
    }
}

void InstancingStarScene::update() {
    for (auto& object : m_objects) {
        object->tick();
    }
}
