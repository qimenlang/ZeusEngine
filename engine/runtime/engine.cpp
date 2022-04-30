#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "engine.h"
namespace zeus{
    engine::engine() {};
    engine::~engine(){};
    void engine::run(){
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        while (true)
        {
            std::cout<<"engine runing..."<<std::endl;
        }
    }
}