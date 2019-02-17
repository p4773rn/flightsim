#include "mesh.h"
#include <iostream>

Mesh::Mesh(const std::vector<Vertex>& vertices, Material&& material) :
    material{std::move(material)}
{
    std::vector<int> indices;
    for (size_t i = 0; i < vertices.size(); ++i)
        indices.push_back(i);
    num_indices = indices.size();
	
	
	//TODO: I red that it is more efficient to reuse vaos, instead of generating new ones
	glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
	glGenBuffers(1, &vbo);
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	// normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	// texcoords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoords));
	glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
}

void Mesh::render(const glm::vec3& camera_pos) {
    Shader& shader = get_shader();
    
    bool useTexture = material.texture != nullptr;
    shader.set("use_texture", useTexture);
    
    if (useTexture) {
        material.texture->activate(GL_TEXTURE0);
    } else {
        shader.set("diffuse_color", material.diffuseColor);
    }

    shader.set("shininess", material.shininess);
    
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
