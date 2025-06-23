
#include "CubeScene.h"
#include "Engine.h"
#include "Model.h"
#include "Shader.h"
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

using namespace std;

#define PRINTAPI(x) std::cout << #x << std::endl;

// float engine.deltaTime() = 0.0f; // 当前帧与上一帧的时间差
// float lastFrame = 0.0f; // 上一帧的时间

float lastX = Zeus::SCR_WIDTH / 2.0f;
float lastY = Zeus::SCR_HEIGHT / 2.0f;
bool firstMouse = true;

auto &engine = Zeus::Engine::getInstance();

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  std::cout << "framebuffer_size_callback [" << width << "," << height << "]"
            << std::endl;
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  std::cout << "mouse(" << xpos << " , " << ypos << " )" << std::endl;
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  Zeus::Engine::getInstance().camera().ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  Zeus::Engine::getInstance().camera().ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    std::cout << "processInput " << GLFW_KEY_ESCAPE << std::endl;
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    std::cout << "deltaTime: " << engine.deltaTime() << std::endl;
    Zeus::Engine::getInstance().camera().ProcessKeyboard(Camera_Movement::FORWARD, engine.deltaTime());
    std::cout << "Camera Position: " << glm::to_string(Zeus::Engine::getInstance().camera().Position)<< " ,camera :"<<&Zeus::Engine::getInstance().camera()
              << std::endl;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    Zeus::Engine::getInstance().camera().ProcessKeyboard(Camera_Movement::BACKWARD, engine.deltaTime());
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    Zeus::Engine::getInstance().camera().ProcessKeyboard(Camera_Movement::LEFT, engine.deltaTime());
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    Zeus::Engine::getInstance().camera().ProcessKeyboard(Camera_Movement::RIGHT, engine.deltaTime());
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    Zeus::Engine::getInstance().camera().ProcessKeyboard(Camera_Movement::UP, engine.deltaTime());
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    Zeus::Engine::getInstance().camera().ProcessKeyboard(Camera_Movement::DOWN, engine.deltaTime());
}

int main() {
  // init glfw
  std::cout << "Zeus Engine Start" << std::endl;
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create window
  GLFWwindow *window = glfwCreateWindow(Zeus::SCR_WIDTH, Zeus::SCR_HEIGHT,
                                        "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  //  glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  // 捕捉光标，并隐藏，光标不显示，且不会离开窗口
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

#ifdef ZEUS_ROOT_DIR
  PRINTAPI(ZEUS_ROOT_DIR);
  std::cout << ZEUS_ROOT_DIR << std::endl;
#endif

  // build and compile our phongShader program
  // ------------------------------------
  std::string vs_path = std::string(ZEUS_ROOT_DIR).append("/shader/default.vs");
  std::string ls_path = std::string(ZEUS_ROOT_DIR).append("/shader/light.fs");
  std::string phong_path =
      std::string(ZEUS_ROOT_DIR).append("/shader/phong.fs");
  std::string phong_sample_path =
      std::string(ZEUS_ROOT_DIR).append("/shader/phongSampleColor.fs");

  std::shared_ptr<Shader> LightShader =
      std::make_shared<Shader>(vs_path.c_str(), ls_path.c_str());
  LightShader->use();
  std::shared_ptr<Shader> phongShader =
      std::make_shared<Shader>(vs_path.c_str(), phong_path.c_str());
  phongShader->use();
  std::shared_ptr<Shader> phongSampleShader =
      std::make_shared<Shader>(vs_path.c_str(), phong_sample_path.c_str());
  phongSampleShader->use();

  Zeus::Engine::getInstance().camera().MouseSensitivity = 0.01f;

  std::string lightCubePath =
      std::string(ZEUS_ROOT_DIR).append("/model/cube.obj");
  Model lightCube(lightCubePath.c_str());
  lightCube.setShader(LightShader);

  std::string DragonPath =
      std::string(ZEUS_ROOT_DIR).append("/model/dragon/Dragon 2.5_fbx.fbx");
  Model dragon(DragonPath.c_str());
  dragon.setShader(phongShader);
  std::cout << "Model Load :" << DragonPath << std::endl;

  std::string cubePath =
      std::string(ZEUS_ROOT_DIR).append("/model/cubeWithNormal.obj");
  Model cube(cubePath.c_str());
  cube.setShader(phongSampleShader);
  Model cube1(cubePath.c_str());
  cube1.setShader(phongSampleShader);
  Model cube2(cubePath.c_str());
  cube2.setShader(phongSampleShader);
  Model cube3(cubePath.c_str());
  cube3.setShader(phongSampleShader);
  Model cube4(cubePath.c_str());
  cube4.setShader(phongSampleShader);

  CubeScene cubeScene;
  cubeScene.init();
  glEnable(GL_DEPTH_TEST);
  // render loop
  while (!glfwWindowShouldClose(window)) {
    // float currentTime = glfwGetTime();
    // engine.deltaTime() = currentTime - lastFrame;
    // lastFrame = currentTime;
    // logic
    engine.update();
    // inputd
    processInput(window);
    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cubeScene.update();

    // auto lightColor = glm::vec3{1.0};
    // LightShader->use();
    // LightShader->setVec3("lightColor", lightColor);

    // lightCube.setPosition(glm::vec3{2 * sin(engine.currentTime()),
    // 0.0, 2.0}); lightCube.setRotation(glm::vec3(1.f, 0.0f, 0.0f), 0.f);
    // lightCube.setScale(glm::vec3(0.04f));
    // lightCube.Draw();

    // be sure to activate the phongShader
    // phongShader->use();
    // phongShader->setVec3("viewPos", Zeus::Engine::getInstance().camera().Position);
    // phongShader->setVec3("light.position", lightCube.transform().position);
    // phongShader->setVec3("light.ambient", glm::vec3{0.2f});
    // phongShader->setVec3("light.diffuse", glm::vec3{1.0f});
    // phongShader->setVec3("light.specular", glm::vec3{1.0f});
    // phongShader->setVec3("material.specular", glm::vec3{0.5f});
    // phongShader->setFloat("material.shininess", 32);

    // dragon.setPosition(glm::vec3{0, 1, 0});
    // dragon.setRotation(glm::vec3(1.f, 0.0f, 0.0f), 180.f);
    // dragon.setScale(glm::vec3{0.01});
    // dragon.Draw();

    // check poll events & swap buffer
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  glfwTerminate();
  return 0;
}