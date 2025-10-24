#pragma once

#include <function/framework/scene/Scene.h>
#include <function/render/ComputeShader.h>
#include <resource/geometries/Geometry.h>

#include <map>

class Object;

class ComputerShaderScene : public Scene {
   private:
    /* data */
    std::unique_ptr<Object> m_screen_quad;
    std::unique_ptr<ComputeShader> m_computeShader;

    int m_counter = 0;

    GLuint m_vertex_ssbo;
    GLuint m_indices_ssbo;
    std::vector<Geometry> m_render_geometries;

    GLuint m_debug_ssbo;

    std::vector<float> m_debug_data = {0, 0, 0, 0, 0, 0, 0, 0};

   public:
    ComputerShaderScene(/* args */) {};
    ~ComputerShaderScene() {};
    void init() override;
    void update() override;
};
