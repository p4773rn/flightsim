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
	glBindVertexArray(0);
	std::cout << "Arrow was created...\n";
}

void Arrow::set_arrows(const std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& debug_arrows, 
                       float camera_distance) {
    this->debug_arrows = debug_arrows;
    this->camera_distance = camera_distance;
}

void Arrow::geometry_pass(const glm::vec3& camera_pos,
                   const glm::mat4& projection,
                   const glm::mat4& view){
	shader.use();

	glBindVertexArray(VAO);
	shader.set("view", view);
	shader.set("projection", projection);
	shader.set("cam_distance", camera_distance);

    for (auto& a : debug_arrows) {
        glm::vec3 origin, direction, color;
        std::tie(origin, direction, color) = a;
        
        float angle = acos(glm::dot(glm::normalize(direction), arrow_dir));
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(origin.x, origin.y, origin.z));
        if (angle != 0)	model = glm::rotate(model, angle, glm::cross(arrow_dir, glm::normalize(direction)));
        
        shader.set("model", model);
        shader.set("scale", glm::length(direction));
        shader.set("color", color);
        
        glDrawElements(GL_TRIANGLES, N_INDICES, GL_UNSIGNED_INT, 0);
    }
	glBindVertexArray(0);
}

Arrow::~Arrow() {
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (EBO) glDeleteBuffers(1, &EBO);
}
