#ifndef AZMESH
#define AZMESH
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <ostream>
#include "../shaders/shader.h"
#include <memory>
#include "texture.h"


struct Vertex{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;
};

struct Material{
    Material() = default;
    Material(const Material& other) = delete;
    Material& operator=(const Material other) = delete;
    Material(Material&& other) = default;
    Material& operator=(Material&& other) = default;

    glm::vec3 diffuseColor;
    std::unique_ptr<Texture> texture;
    float shininess = 32;
    float alpha = 1; 
};

class Mesh{
public:
	Mesh(const std::vector<Vertex>& vertices, Material&& material);
	Mesh(const Mesh& other) = delete;
	Mesh& operator=(const Mesh& other) = delete;
	Mesh(Mesh&& other);
	//Mesh(const std::vector<Vertex>& vectors, const std::vector<unsigned int>& indices);
	~Mesh();

    bool transparent() const { return material.alpha != 1; }
    void sortByZ(glm::mat4 model, glm::vec3 cameraPos);

	void setup();
	void draw(Shader& shader);
private:
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	Material material;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};


std::ostream& operator << (std::ostream& stream, const Vertex& v);

#endif
