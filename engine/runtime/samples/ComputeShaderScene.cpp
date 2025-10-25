#include "ComputeShaderScene.h"

#include <resource/geometries/QuadGeometry.h>
#include <resource/geometries/SphereGeometry.h>

#include "Engine.h"

const unsigned int TEXTURE_zWIDTH = 1000;
const unsigned int TEXTURE_HEIGHT = 1000;

// 根据目标平台设置已知的 warp/wavefront 大小
#ifdef TARGET_NVIDIA
const int WARP_SIZE = 32;  // NVIDIA GPU 通常为 32
#elif defined TARGET_AMD
const int WARP_SIZE = 64;  // 大多数 AMD GPU 为 64 (较新的 RDNA 架构也有 32)
#elif defined TARGET_APPLE  // Apple Silicon
const int WARP_SIZE = 32;  // 通常为 32
#else
const int WARP_SIZE = 32;  // 安全默认值
#endif

// TODO 重构到基础功能
void queryBufferLayout(GLuint program) {
    GLint blockIndex = glGetProgramResourceIndex(
        program, GL_SHADER_STORAGE_BLOCK, "VertexBuffer");
    if (blockIndex == GL_INVALID_INDEX) {
        std::cout << "Not find VertexBuffer block" << std::endl;
        return;
    }

    GLenum props[] = {
        GL_BUFFER_DATA_SIZE,  // 数据大小
    };

    // 获取存储块信息
    GLint blockSize;
    glGetProgramResourceiv(program, GL_SHADER_STORAGE_BLOCK, blockIndex, 1,
                           props, 1, nullptr, &blockSize);

    std::cout << "存储块大小：" << blockSize << " 字节" << std::endl;

    // 获取成员数量
    GLint numMembers;
    glGetProgramInterfaceiv(program, GL_BUFFER_VARIABLE, GL_ACTIVE_RESOURCES,
                            &numMembers);

    // 查询每个成员的信息
    for (int i = 0; i < numMembers; ++i) {
        const GLenum props[] = {GL_NAME_LENGTH, GL_TYPE, GL_OFFSET,
                                GL_ARRAY_SIZE};
        GLint values[4];

        glGetProgramResourceiv(program, GL_BUFFER_VARIABLE, i, 4, props, 4,
                               nullptr, values);

        // 获取成员名称
        std::vector<char> name(values[0] + 1);
        glGetProgramResourceName(program, GL_BUFFER_VARIABLE, i, name.size(),
                                 nullptr, name.data());

        std::cout << "成员: " << name.data() << ", 偏移: " << values[2]
                  << ", 类型: " << values[1] << ", 数组大小: " << values[3]
                  << std::endl;
    }
}

void ComputerShaderScene::init() {
    std::string quad_vs_path =
        std::string(ZEUS_ROOT_DIR)
            .append("/shader/computerShader/screenQuad.vs");
    std::string quad_fs_path =
        std::string(ZEUS_ROOT_DIR)
            .append("/shader/computerShader/screenQuad.fs");
    m_computeShader = std::make_unique<ComputeShader>(
        std::string(ZEUS_ROOT_DIR)
            .append("/shader/computerShader/computeShader.cs"));

    auto quad_mat =
        Material::create(quad_vs_path.c_str(), quad_fs_path.c_str());

    quad_mat->shader()->use();
    quad_mat->shader()->setInt("tex", 0);

    unsigned int render_tex;
    glGenTextures(1, &render_tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, render_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, Zeus::SCR_WIDTH,
                 Zeus::SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
    // 绑定纹理图片，不含滤波、mipmap;以便着色器使用image2D等图像变量来随机、原子地读写纹理数据;
    glBindImageTexture(0, render_tex, 0, GL_FALSE, 0, GL_READ_WRITE,
                       GL_RGBA32F);

    // objects
    auto sphere = SphereGeometry::create(0.5f);
    // auto sphere = QuadGeometry::getDefault(QuadGeometryType::ScreenQuad);
    // auto sphere = QuadGeometry::getDefault(QuadGeometryType::NormalQuad);

    auto& vertices = sphere.vertices();
    auto& indices = sphere.indices();
    std::cout << vertices.size() << std::endl;
    std::cout << indices.size() << std::endl;
    std::cout << "Position :" << offsetof(Vertex, Position) << std::endl;
    std::cout << "Normal :" << offsetof(Vertex, Normal) << std::endl;
    std::cout << "TexCoords :" << offsetof(Vertex, TexCoords) << std::endl;
    std::cout << "Tangent :" << offsetof(Vertex, Tangent) << std::endl;
    std::cout << "Bitangent :" << offsetof(Vertex, Bitangent) << std::endl;
    std::cout << "m_BoneIDs :" << offsetof(Vertex, m_BoneIDs) << std::endl;
    std::cout << "m_Weights :" << offsetof(Vertex, m_Weights) << std::endl;
    std::cout << sizeof(Vertex) << std::endl;

    glGenBuffers(1, &m_vertex_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_vertex_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, vertices.size() * sizeof(Vertex),
                 vertices.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_vertex_ssbo);

    glGenBuffers(1, &m_indices_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_indices_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
                 indices.size() * sizeof(unsigned int), indices.data(),
                 GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_indices_ssbo);

    glGenBuffers(1, &m_debug_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_debug_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_debug_data.size() * sizeof(int),
                 m_debug_data.data(), GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_debug_ssbo);

    // screen quad
    m_screen_quad = std::make_unique<Object>(quad_mat);
    Primitive quadPrimitive(
        QuadGeometry::getDefault(QuadGeometryType::ScreenQuad),
        quad_mat->defaultInstance()->duplicate());
    m_screen_quad->addComponent<MeshComponent>(PrimitiveList{quadPrimitive});

    int count[3];
    int invocations;
    int size[3];
    // work group count
    // 一次Dispatch调用（一次计算调度）中,在每个维度上最多定义多少工作组;
    // work group size 三个维度上invocation的最大值;
    // size.x*size.y*size.z <= invocations;
    for (int i = 0; i < 3; i++) {
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, i, &count[i]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, i, &size[i]);
    }
    std::cout << "limits Work Group Count : x " << count[0] << ",y:" << count[1]
              << ",z:" << count[2] << std::endl;
    std::cout << "limits Work Group size : x " << size[0] << ",y:" << size[1]
              << ",z:" << size[2] << std::endl;
    // 每个work group中invocation的数量限制;
    // 即使每个维度的限制很高，但三者乘法的总和也不能超过 invocations;
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &invocations);
    std::cout << "limits Work Group invoations : " << invocations << std::endl;

    std::cout << "warp size : " << WARP_SIZE << std::endl;

    queryBufferLayout(m_computeShader->ID);
}

void ComputerShaderScene::update() {
    float currentFrame = Zeus::Engine::getInstance().currentTime();
    m_computeShader->use();
    m_computeShader->setFloat("t", currentFrame);
    m_computeShader->setVec3("camera.world_position",
                             Zeus::Engine::getInstance().camera().position());
    m_computeShader->setVec3("camera.world_front",
                             Zeus::Engine::getInstance().camera().front());
    m_computeShader->setVec3("camera.world_up",
                             Zeus::Engine::getInstance().camera().up());
    m_computeShader->setVec3("camera.world_right",
                             Zeus::Engine::getInstance().camera().right());
    m_computeShader->setFloat(
        "camera.v_fov", Zeus::Engine::getInstance().camera().pjt_para().FOV);
    m_computeShader->setFloat(
        "camera.aspect_ratio",
        Zeus::Engine::getInstance().camera().pjt_para().AspectRatio);

    glDispatchCompute(Zeus::SCR_WIDTH / 8, Zeus::SCR_HEIGHT / 8, 1);
    // 内存屏障，确保computeshader中Image相关计算结果全部写入内存,
    // 及读取debug信息
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT |
                    GL_SHADER_STORAGE_BARRIER_BIT |
                    GL_BUFFER_UPDATE_BARRIER_BIT);
    // 读取CS调试数据
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_debug_ssbo);
    float* countPtr =
        (float*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    float debugSize = countPtr[0];
    std::cout << "debug data size:" << debugSize << std::endl;
    for (int i = 0; i < m_debug_data.size(); i++) {
        std::cout << "debug " << i << "data:" << countPtr[i] << std::endl;
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto& m_quadShader = m_screen_quad->getComponent<MeshComponent>()
                             ->primitives()[0]
                             .matInstance;
    m_quadShader->use();
    m_screen_quad->tick();
}
