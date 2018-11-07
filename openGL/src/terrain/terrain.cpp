#include "terrain.h"
#include <vector>
#include <utility>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Terrain::Terrain(const std::string& fname, const glm::vec3& camera_position) : 
	terrain_renderer{camera_position, 2000.0f, 2000.0f}
{
	//terrain_renderer = Quadtree(camera_position, 2000.0f, 2000.0f);
}

void Terrain::draw(const glm::vec3& camera_position, const glm::mat4& view, const glm::mat4& proj) {
	/*shader->use();
	glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "view"),
                     1, GL_FALSE, glm::value_ptr(view));
  	glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "projection"),
                     1, GL_FALSE,
                     glm::value_ptr(proj));
  	glBindVertexArray(VAO);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_PATCHES, 0, 4*patches*patches);*/
	terrain_renderer.render(camera_position, view, proj);
}

void Terrain::setup() {
}
