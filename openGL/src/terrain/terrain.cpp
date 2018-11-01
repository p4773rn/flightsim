#include "terrain.h"
#include <vector>
#include <utility>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Terrain::Terrain(const std::string& fname) {
	if (!map.loadFromFile(fname)) {
		std::cerr << "Error while loading file:" << fname << std::endl;
		return;
	}
	map_text.loadFromFile("assets/terrain/text.tga");
	std::cout << map.getSize().x << " " << map.getSize().y << std::endl;

	std::vector<std::pair<std::string, GLuint>> paths = {
      std::pair<std::string, GLuint>("shaders/terrain.vrtx", GL_VERTEX_SHADER),
      std::pair<std::string, GLuint>("shaders/terrain.tec", GL_TESS_CONTROL_SHADER),
      std::pair<std::string, GLuint>("shaders/terrain.tse", GL_TESS_EVALUATION_SHADER),
      std::pair<std::string, GLuint>("shaders/terrain.frgmnt", GL_FRAGMENT_SHADER)
    };
    shader = std::make_unique<Shader>(paths);
    patches = 16;
    setup();
}

Terrain::~Terrain() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Terrain::draw(const glm::mat4& view, const glm::mat4& proj) {
	shader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hmap_texture);
	glUniform1i(glGetUniformLocation(shader->getID(), "hmap"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glUniform1i(glGetUniformLocation(shader->getID(), "text"), 1);

	glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "model"),
                     1, GL_FALSE, glm::value_ptr(getMVP()));
	glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "view"),
                     1, GL_FALSE, glm::value_ptr(view));
  	glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "projection"),
                     1, GL_FALSE,
                     glm::value_ptr(proj));
  	glBindVertexArray(VAO);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_PATCHES, 0, 4*patches*patches);
}

glm::mat4 Terrain::getMVP() {
	float c = map.getSize().x;
	glm::mat4 scale = glm::mat4(1.0f);
	scale = glm::rotate(scale, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
	return glm::scale(scale, glm::vec3(c,c,200));
}

void Terrain::setup() {
	shader->use();
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	GLfloat vertices[4*patches*patches][2];
	float x = -1.0f;
	float y = 1.0f + 2.0f/float(patches);
	for (unsigned int i = 0; i < 4*patches*patches; i += 4){
		if ( i % (4*patches) == 0){
			x = -1.0f;
			y -= 2.0f/float(patches);
		}
		vertices[i][0] = vertices[i+3][0] = x;
		vertices[i+1][0] = vertices[i+2][0] = x + 2.0f/float(patches);
		vertices[i][1] = vertices[i+1][1] = y;
		vertices[i+3][1] = vertices[i+2][1] = y - 2.0f/float(patches);
		x += 2.0f/float(patches);
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 4*patches*patches*2*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (void*) 0);
	glEnableVertexAttribArray(0);

	glGenTextures(1, &hmap_texture);
	glBindTexture(GL_TEXTURE_2D, hmap_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				map.getSize().x, map.getSize().y,
				0, GL_RGBA, GL_UNSIGNED_BYTE, map.getPixelsPtr());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &tex1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				map_text.getSize().x, map_text.getSize().y,
				0, GL_RGBA, GL_UNSIGNED_BYTE, map_text.getPixelsPtr());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
