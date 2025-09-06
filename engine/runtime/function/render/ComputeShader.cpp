#include "ComputeShader.h"

#include "include/glad/glad.h"  // include glad to get all the required OpenGL headers

ComputeShader::ComputeShader(std::string path) : ComputeShader(path.c_str()) {}

ComputeShader::ComputeShader(const char *path) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string shaderCode;
    std::ifstream shaderFile;
    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        shaderFile.open(path);
        std::stringstream cShaderStream;
        // read file's buffer contents into streams
        cShaderStream << shaderFile.rdbuf();
        // close file handlers
        shaderFile.close();
        // convert stream into string
        shaderCode = cShaderStream.str();
    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::ComputeShader::FILE_NOT_SUCCESSFULLY_READ: "
                  << e.what() << std::endl;
    }
    const char *cShaderCode = shaderCode.c_str();
    // 2. compile shaders
    unsigned int compute;
    // vertex shader
    compute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute, 1, &cShaderCode, NULL);
    glCompileShader(compute);
    checkCompileErrors(compute, "COMPUTE");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, compute);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no
    // longer necessary
    glDeleteShader(compute);
}
