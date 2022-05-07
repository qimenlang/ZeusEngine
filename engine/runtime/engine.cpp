#include "engine.h"

#include <iostream>
#include <cassert>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <Eigen/Dense>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "ConfigManager.h"
#include "function/render/camera.h"

using Eigen::MatrixXd;
// get compile var form cmake , why 2 macro func ???
#define GET_ZEUS_STR(s) GET_STR(s)
#define GET_STR(s) #s

 // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------

float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

GLFWwindow* window;
Shader default_shader;
Camera camera;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


float euler_2_radius(float euler)
{   
    return euler * 0.01745329251994329576923690768489;
}

Eigen::Matrix4f translate(const Eigen::Matrix4f& model,const Eigen::Vector3f& tranlation)
{
    Eigen::Affine3f target = Eigen::Affine3f::Identity();
    target.translate(tranlation);
    return target.matrix() * model;
}
Eigen::Matrix4f rotate(const Eigen::Matrix4f& model, float euler,Eigen::Vector3f axis)
{
    axis.normalize();
    Eigen::Affine3f target = Eigen::Affine3f(Eigen::AngleAxisf(euler_2_radius(euler), axis));
    target.rotate(Eigen::AngleAxisf(euler_2_radius(euler), axis));
    return target.matrix() * model;
}

Eigen::Matrix4f scale(const Eigen::Matrix4f& model, const Eigen::Vector3f& scale)
{
    Eigen::Affine3f target = Eigen::Affine3f::Identity();
    target.scale(scale);
    return target.matrix() * model;
}

// Affine3d is a 4*4 Homogeneous matrix
Eigen::Matrix4f create_rotation_matrix(float euler, Eigen::Vector3f axis) {
    axis.normalize();
    Eigen::Affine3f rotate = Eigen::Affine3f(Eigen::AngleAxisf(euler_2_radius(euler), axis));
    return rotate.matrix();
}

Eigen::Matrix4f create_translation_matrix(float tx, float ty, float tz)
{
    Eigen::Affine3f translation(Eigen::Translation3f(Eigen::Vector3f(tx, ty, tz)));
    return translation.matrix();
}

Eigen::Matrix4f create_projection_martrix(float fovy,float aspect,float zNear, float zFar)
{
    Eigen::Transform<float, 3, Eigen::Projective> tr;
    tr.matrix().setZero();
    assert(aspect > 0);
    assert(zFar > zNear);
    assert(zNear > 0);
    double radf = euler_2_radius(fovy);
    double tan_half_fovy = std::tan(radf / 2.0);
    tr(0, 0) = 1.0 / (aspect * tan_half_fovy);
    tr(1, 1) = 1.0 / (tan_half_fovy);
    tr(2, 2) = -(zFar + zNear) / (zFar - zNear);
    tr(3, 2) = -1.0;
    tr(2, 3) = -(2.0 * zFar * zNear) / (zFar - zNear);

    return tr.matrix();
}
namespace zeus{
    engine::engine() {}
    void engine::LogicTick(float delta_time)
    {
    }
    void engine::RenderTick()
    {
        processInput(window);
        //rendering 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // draw our first triangle
        default_shader.use();

        float roll, yaw, pitch = 0.0;
        float rotate_euler = (float)glfwGetTime() * 50.0;

        const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;

        camera.LookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 view = camera.GetViewMatrix();

        float aspect = float(window_width) / window_height;
        camera.SetPerspective(45.0, aspect, 0.1, 100.0);
        glm::mat4 projection = camera.GetProjectionMatrix();

        default_shader.setMat4("view", glm::value_ptr(view));
        default_shader.setMat4("projection", glm::value_ptr(projection));

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        for (int i = 0; i < 10; i++) {
            auto pos = cubePositions[i];

            float rotate_euler = (float)glfwGetTime() * 20.0;
            Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
            model = rotate(model, rotate_euler, Eigen::Vector3f(1.0, 1.0, 1.0));
            model = translate(model, Eigen::Vector3f(pos.x, pos.y, pos.z));

            default_shader.setMat4("model", model.data());
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);

        }

        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();// checks if any events are triggered,updates the window state, and calls the corresponding functions 
    };
    engine::~engine(){};
    void engine::init()
    {
        // init glfw window
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window = glfwCreateWindow(window_width, window_height, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        //load glad
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
        // vertex shader
        auto shader_folder = zeus::ConfigManager::instance().getShaderFolder();
        std::string vs_path = shader_folder.string() + "\\default.vs";
        std::string fs_path = shader_folder.string() + "\\default.fs";
        default_shader.load(vs_path.c_str(), fs_path.c_str());

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // uv attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        // uncomment this call to draw in wireframe polygons.
        glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);

        // apply texture
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        auto texture_folder = zeus::ConfigManager::instance().getTextureFolder();
        std::string texture_file = texture_folder.string() + "\\container.jpg";

        unsigned char* data = stbi_load(texture_file.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        glEnable(GL_DEPTH_TEST);
    }
    void engine::run() {
        while (!glfwWindowShouldClose(window))
        {
            float delta_time;
            {       
                using namespace std::chrono;
                steady_clock::time_point time_now = steady_clock::now();
                duration<float> time_span = time_now - time_last_frame;
                delta_time = time_span.count(); 
            }
            LogicTick();
            RenderTick();
        }
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glfwTerminate();
    }
    

}