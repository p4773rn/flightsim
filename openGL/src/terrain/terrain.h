#ifndef C_TERRAIN_FS
#define C_TERRAIN_FS
#include <string>
#include <memory>
#include "../shaders/shader.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>

class Terrain {
private:
	sf::Image map;
	sf::Image map_text;
	unsigned int hmap_texture, tex1;
	unsigned int patches;
	double scale;
	GLuint VAO, VBO;
	Shader* shader;
public:
	Terrain(const std::string& fname);
	void draw(const glm::mat4& view, const glm::mat4& proj);
	glm::mat4 getMVP();
	void setup();
};
#endif