#include "grid3d.h"
#include <cmath>
#include <SFML/Graphics/Image.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <tuple>

#define HALF_SIZE 8000
#define Y_INCREMENT 0

Grid3d::Grid3d() : 
	shader({{"src/openGL/shaders/grid3d.vrtx", GL_VERTEX_SHADER},
			{"src/openGL/shaders/grid3d.frgmnt", GL_FRAGMENT_SHADER}})
{
    vertices = {
         HALF_SIZE, -Y_INCREMENT, HALF_SIZE,
         HALF_SIZE, -Y_INCREMENT,-HALF_SIZE,
        -HALF_SIZE, -Y_INCREMENT, HALF_SIZE,
        -HALF_SIZE, -Y_INCREMENT,-HALF_SIZE
    };
    indices = {3, 1, 2, 1, 0, 2};


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
}

Grid3d::~Grid3d() {
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (EBO) glDeleteBuffers(1, &EBO);
}

void Grid3d::render(const glm::vec3& pos, const glm::mat4& view, const glm::mat4& projection) {
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(VAO);
	//glDepthMask(GL_FALSE);
	shader.use();
	glm::mat4 model1 = glm::translate(glm::mat4(1.), 
	                    glm::vec3(  pos.x, 
	                                0, //pos.y, //int(pos.y/Y_INCREMENT) * Y_INCREMENT, 
	                                pos.z));
	glm::mat4 model2 = glm::translate(glm::mat4(1.), 
	                    glm::vec3(  pos.x, 
	                                0,
	                                pos.z));

	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model1"),
                     1, GL_FALSE, glm::value_ptr(model1));
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model2"),
                     1, GL_FALSE, glm::value_ptr(model2));
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "view"),
                     1, GL_FALSE, glm::value_ptr(view));
  	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "projection"),
                     1, GL_FALSE,
                     glm::value_ptr(projection));
    glUniform1f(glGetUniformLocation(shader.getID(), "height"), pos.y);
    

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glDepthMask(GL_TRUE);
}


