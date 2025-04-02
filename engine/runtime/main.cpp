
#include "Camera.h"
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
// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

Camera camera(cameraPos, cameraUp);

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

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    std::cout << "processInput " << GLFW_KEY_ESCAPE << std::endl;
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
}

int main() {
  // init glfw
  std::cout << "Zeus Engine Start" << std::endl;
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create window
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
  std::string vs_path = std::string(ZEUS_ROOT_DIR).append("/shader/shader.vs");
  std::string fs_path = std::string(ZEUS_ROOT_DIR).append("/shader/shader.fs");
  std::string ls_path = std::string(ZEUS_ROOT_DIR).append("/shader/light.fs");

  std::cout << vs_path << std::endl;

  Shader phongShader(vs_path.c_str(), fs_path.c_str());
  phongShader.use();
  Shader LightShader(vs_path.c_str(), ls_path.c_str());
  LightShader.use();

  camera.MouseSensitivity = 0.01f;

  std::string DragonPath =
      std::string(ZEUS_ROOT_DIR).append("/model/dragon/Dragon 2.5_fbx.fbx");

  Model dragon(DragonPath.c_str());

  std::string lightCubePath =
      std::string(ZEUS_ROOT_DIR).append("/model/cube.obj");
  Model lightCube(lightCubePath.c_str());

  std::cout << "Model Load :" << DragonPath << std::endl;

  glEnable(GL_DEPTH_TEST);
  // render loop
  while (!glfwWindowShouldClose(window)) {
    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;
    // input
    processInput(window);

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 lightColor = glm::vec3{0.7};

    // be sure to activate the phongShader
    phongShader.use();
    std::cout << "Model default transform :"
              << glm::to_string(dragon.transform().position) << ","
              << glm::to_string(dragon.transform().rotateAxis) << ","
              << dragon.transform().rotation << ","
              << glm::to_string(dragon.transform().scale) << std::endl;
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    phongShader.setMat4("view", view);
    phongShader.setMat4("projection", projection);
    phongShader.setVec3("lightPos", lightCube.transform().position);
    phongShader.setVec3("lightColor", lightColor);

    dragon.setPosition(glm::vec3{0, 0, 0});
    dragon.setRotation(glm::vec3(1.f, 0.0f, 0.0f), 180.f);
    dragon.setScale(glm::vec3{0.01});
    dragon.Draw(phongShader);

    LightShader.use();
    LightShader.setMat4("view", view);
    LightShader.setMat4("projection", projection);
    LightShader.setVec3("lightColor", lightColor);

    lightCube.setPosition(glm::vec3{1, 0, 0});
    lightCube.setRotation(glm::vec3(1.f, 0.0f, 0.0f), 0.f);
    lightCube.setScale(glm::vec3(0.1f));
    lightCube.Draw(LightShader);

    // check poll events & swap buffer
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  glfwTerminate();
  return 0;
}