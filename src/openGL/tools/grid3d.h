#ifndef GRID3D_H
#define GRID3D_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../shaders/shader.h"
#include <memory>
#include "../renderer.h"

class Grid3d : public Renderable{
public:
	Grid3d();
	~Grid3d();
	void geometry_pass(const glm::vec3& camera_pos,
                       const glm::mat4& projection,
                       const glm::mat4& view);
    void set_pos(const glm::vec3& pos) { this->pos = pos; }
private:
    glm::vec3 pos;

	Shader shader;
	GLuint VAO;
	GLuint EBO;
	GLuint VBO;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;
};
#endif
