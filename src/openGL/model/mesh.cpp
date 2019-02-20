#include "mesh.h"
#include <iostream>
#include "../misc.h"
#include <glm/gtc/type_ptr.hpp>
#include <cassert>
#include <algorithm>

using std::cout;
using std::endl;

Mesh::Mesh(const std::vector<Vertex>& _vertices, Material&& material) :
	vertices{_vertices}, material{std::move(material)}
{
    for (unsigned int i = 0; i < vertices.size(); ++i)
        indices.push_back(i);

	setup();
}

Mesh::Mesh(Mesh&& other) :
    vertices{std::move(other.vertices)}, indices{std::move(other.indices)},
    vao{other.vao}, vbo{other.vbo}, ebo{other.ebo}, 
    material{std::move(other.material)} 
{
    other.vao = 0;
    other.vbo = 0;
    other.ebo = 0;
}


Mesh::~Mesh() {
    if (vao) {
        glDeleteVertexArrays(1, &vao);
    }
    if (vbo) {
        glDeleteBuffers(1, &vbo);
    }
}

void Mesh::setup(){
	glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
	glGenBuffers(1, &vbo);
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

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

void Mesh::draw(Shader& shader){
    bool useTexture = material.texture != nullptr;

    glUniform1i(glGetUniformLocation(shader.getID(), "useTexture"), useTexture);
    
    if (useTexture) {
        material.texture->activate(GL_TEXTURE0);
    } else {
        glUniform3fv(glGetUniformLocation(shader.getID(), "diffuseColor"),
                     1, glm::value_ptr(material.diffuseColor));
    }

    glUniform1f(glGetUniformLocation(shader.getID(), "shininess"), material.shininess);
    glUniform1f(glGetUniformLocation(shader.getID(), "alpha"), material.alpha);


    if (transparent()) {
        
    }


	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void Mesh::sortByZ(glm::mat4 model, glm::vec3 cameraPos) {
    assert(indices.size() % 3 == 0);
    int numFaces = indices.size() / 3;
    float dist[numFaces];
    
    for (unsigned int f = 0; f < numFaces; ++f) {
        auto& v1 = vertices[indices[f * 3]].position;
        auto& v2 = vertices[indices[f * 3 + 1]].position;
        auto& v3 = vertices[indices[f * 3 + 2]].position;

        auto center = (v1 + v2 + v3) / 3.0f;
        center = glm::vec3(model * glm::vec4(center, 1));
        dist[f] = -glm::length(cameraPos - center);
    }

    unsigned int fs[numFaces];
    for (unsigned int f = 0; f < numFaces; ++f)
        fs[f] = f;

    std::sort( fs, fs + numFaces,
               [&](unsigned int f1, unsigned int f2){return dist[f1] < dist[f2];} );
    
    std::vector<unsigned int> indices_(indices.size());
    for (unsigned int i = 0; i < numFaces; ++i) {
        indices_[i * 3] = indices[fs[i] * 3];
        indices_[i * 3 + 1] = indices[fs[i] * 3 + 1];
        indices_[i * 3 + 2] = indices[fs[i] * 3 + 2];
    }
    indices = indices_;
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}


std::ostream& operator << (std::ostream& stream, const Vertex& v) {
    stream << "<" << v.position << ", " << v.normal << ", " << v.texcoords << ">";
    return stream;
}
