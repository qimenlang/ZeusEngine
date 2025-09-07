#include "ComputeShaderScene.h"

#include <resource/geometries/QuadGeometry.h>

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

    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, Zeus::SCR_WIDTH,
                 Zeus::SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
    // 绑定纹理图片，不含滤波、mipmap;以便着色器使用image2D等图像变量来随机、原子地读写纹理数据;
    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    m_quad = std::make_unique<Object>(quad_mat);
    Primitive quadPrimitive(
        QuadGeometry::getDefault(QuadGeometryType::ScreenQuad),
        quad_mat->defaultInstance()->duplicate());
    m_quad->addComponent<MeshComponent>(PrimitiveList{quadPrimitive});

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
}

void ComputerShaderScene::update() {
    float currentFrame = Zeus::Engine::getInstance().currentTime();
    m_computeShader->use();
    m_computeShader->setFloat("t", currentFrame);

    glDispatchCompute(Zeus::SCR_WIDTH / 8, Zeus::SCR_HEIGHT / 8, 1);
    // 内存屏障，确保compute shader中Image相关计算结果全部写入内存
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto &m_quadShader =
        m_quad->getComponent<MeshComponent>()->primitives()[0].matInstance;
    m_quadShader->use();
    m_quad->tick();
}
