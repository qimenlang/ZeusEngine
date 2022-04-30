#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "engine.h"

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
namespace zeus{
    engine::engine() {};
    engine::~engine(){};
    void engine::run(){

        // init glfw window
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);

        //load glad
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }

        //viewport ,location and size  
        glViewport(0, 0, 800, 600);

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        while (!glfwWindowShouldClose(window))
        {
            std::cout<<"engine runing..."<<std::endl;
            processInput(window);

            //rendering 
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // swap buffers and poll IO events
            glfwSwapBuffers(window);
            glfwPollEvents();// checks if any events are triggered,updates the window state, and calls the corresponding functions 
        }

        glfwTerminate();
    }
}