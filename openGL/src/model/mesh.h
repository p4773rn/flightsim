#ifndef AZMESH
#define AZMESH
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <ostream>

struct Vertex{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;
	GLint materialI;
};

class Mesh{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Mesh(const std::vector<Vertex>& vertices);
	//Mesh(const std::vector<Vertex>& vectors, const std::vector<unsigned int>& indices);
	~Mesh();
	void setup();
	void draw();
private:
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};


std::ostream& operator << (std::ostream& stream, const Vertex& v);

#endif
