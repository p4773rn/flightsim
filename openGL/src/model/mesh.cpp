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

void Mesh::setup(){
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);
	// glGenBuffers(1, &m_ebo);
	Vertex v = vertices[1];
	/*std::cout << v.position.x << ' ' << v.position.y << ' ' << v.position.z << ' '
		  << v.normal.x << ' ' << v.normal.y << ' ' << v.normal.z << std::endl;*/
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);
}

void Mesh::draw(){
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}