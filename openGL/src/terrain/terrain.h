#ifndef C_TERRAIN_FS
#define C_TERRAIN_FS
#include <string>
#include <memory>
#include "../shaders/shader.h"
//#include "../model/texture.h"
#include <glm/glm.hpp>
#include "quadtree.h"
#include <SFML/Graphics/Image.hpp>

// I haven't include any collisions yet
class Terrain {
private:
	double scale;
	Quadtree terrain_renderer;
	sf::Image height_map;
public:
	Terrain(const std::string& fname, const glm::vec3& camera_position);
	void draw(const glm::vec3& camera_position, const glm::mat4& view, const glm::mat4& proj);
};
#endif
