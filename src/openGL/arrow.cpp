#include "arrow.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

const unsigned int N_INDICES = 12 * 3;
const glm::vec3 arrow_dir(0.0f, 1.0f, 0.0f);

Arrow::Arrow() : shader({{"src/openGL/shaders/arrow.vrtx", GL_VERTEX_SHADER},
						{"src/openGL/shaders/arrow.frgmnt", GL_FRAGMENT_SHADER}})
{
	float vertices[] = 
			{-0.25f, 0.0f, 0.0f,
			0.25f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.25f,
			0.0f, 0.0f, -0.25f,

			-0.25f, 0.85f, 0.0f,
			0.25f, 0.85, 0.0f,
			0.0f, 0.85f, 0.25f,
			0.0f, 0.85f, -0.25f,

			0.0f, 1.0f, 0.0f,
			-0.85f, 0.85f, 0.0f,
			0.85f, 0.85f, 0.0f,
			0.0f, 0.85f, -0.85f,
			0.0f, 0.85, 0.85f};
	unsigned int indices[N_INDICES] = {
		0, 2, 4,
		2, 6, 4,
		2, 5, 6,
		2, 1, 5,
		1, 7, 5,
		1, 3, 7,
		3, 0, 7,
		0, 4, 7,
		9, 11, 8,
		11, 10, 8,
		10, 12, 8,
		12, 9, 8	
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	std::cout << "Arrow was created...\n";
}

void Arrow::set_uniforms(const glm::mat4& view, const glm::mat4& projection, float distance) {
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "view"),
                     1, GL_FALSE, glm::value_ptr(view));
  	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "projection"),
                     1, GL_FALSE,
                     glm::value_ptr(projection));
    glUniform1f(glGetUniformLocation(shader.getID(), "cam_distance"), distance);
}

void Arrow::draw(const glm::vec3& origin, const glm::vec3& direction, float scale) {
	shader.use();
	float angle = acos(glm::dot(glm::normalize(direction), arrow_dir));
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(origin.x, origin.y, origin.z));
	if (angle != 0)	model = glm::rotate(model, angle, glm::cross(arrow_dir, glm::normalize(direction)));

	glBindVertexArray(VAO);
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"),
                     1, GL_FALSE, glm::value_ptr(model));
    glUniform1f(glGetUniformLocation(shader.getID(), "scale"), scale);
    
	glDrawElements(GL_TRIANGLES, N_INDICES, GL_UNSIGNED_INT, 0);
}

Arrow::~Arrow() {
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (EBO) glDeleteBuffers(1, &EBO);
}
