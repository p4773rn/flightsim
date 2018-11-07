#include "terrain.h"
#include <vector>
#include <utility>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Terrain::Terrain(const std::string& fname, const glm::vec3& camera_position) :
	terrain_renderer{camera_position, fname, 200.0f, 200.0f}
{
	height_map.loadFromFile(fname);
	terrain_renderer.set_wh(height_map.getSize().x, height_map.getSize().y);
}

void Terrain::draw(const glm::vec3& camera_position, const glm::mat4& view, const glm::mat4& proj) {
	terrain_renderer.render(camera_position, view, proj);
}