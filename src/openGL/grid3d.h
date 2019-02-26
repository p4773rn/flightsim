#ifndef GRID3D_H
#define GRID3D_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shaders/shader.h"
#include <memory>

class Grid3d {
public:
	Grid3d();
	~Grid3d();
	void render(const glm::vec3& pos, const glm::mat4& view, const glm::mat4& projection);
private:
	Shader shader;
	GLuint VAO;
	GLuint EBO;
	GLuint VBO;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;
};
#endif
