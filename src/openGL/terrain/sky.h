#ifndef SKY_CLASS
#define SKY_CLASS
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../shaders/shader.h"

class Sky {
public:
	Sky(const std::string& path);
	void render(const glm::mat4& view, const glm::mat4& projection);
private:
	GLuint texture;
	Shader* shader;
	GLuint VAO;
	GLuint VBO;
};
#endif
