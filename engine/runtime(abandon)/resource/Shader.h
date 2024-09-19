#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader()noexcept = default;
	void load(const char* vertexPath, const char* fragmentPath);
	~Shader() noexcept = default;
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, float* value) const;
	void setVec3(const std::string& name, float* value) const;
	void setVec3(const std::string& name, glm::vec3 vector3) const;
	void setVec3(const std::string& name, float value1,float value2, float value3) const;

	unsigned int ID;
private:
	void checkCompileErrors(unsigned int shader, std::string type);
};
