#include "mesh.h"
#include <iostream>

Mesh::Mesh(const std::vector<Vertex>& _vertices) :
	vertices{_vertices}
{
	setup();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) : 
	vertices{vertices}, indices{indices}
{
	setup();
} 

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Mesh::setup(){
	glGenVertexArrays(1, &vao);
	// glGenBuffers(1, &ebo);
	glGenBuffers(1, &vbo);
	
	//Vertex v = vertices[1];
	/*std::cout << v.position.x << ' ' << v.position.y << ' ' << v.position.z << ' '
		  << v.normal.x << ' ' << v.normal.y << ' ' << v.normal.z << std::endl;*/
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
	glEnableVertexAttribArray(0);
	// normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);
}

void Mesh::draw(){
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
