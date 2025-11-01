#pragma once
#include <glm/glm.hpp>
class Shader {
public:
	unsigned ID;
	Shader(const char* vertex, const char* fragment);
	void use();
	void setInt(const char* name, int value);
	void setFloat(const char* name, float value);
	void setMatrix(const char* name, glm::mat4* value);
};