#ifndef MESH_H
#define MESH_H

#include "../renderer.h"
#include <glm/glm.hpp>
#include "../shaders/shader.h"
#include "texture.h"
#include <memory>

struct Vertex{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;
};

struct Material{
    Material() = default;
    Material(const Material& other) = delete;
    Material& operator=(const Material other) = delete;
    Material(Material&& other) = default;
    Material& operator=(Material&& other) = default;

    glm::vec3 diffuseColor;
    std::unique_ptr<Texture> texture;
    float shininess = 32;
    float alpha = 1; 
};


class Mesh : public Renderable {
public:
    Mesh(const std::vector<Vertex>& vertices, Material&& material);
    Mesh(const Mesh& other) = delete;
    Mesh& operator=(const Mesh other) = delete;
    Mesh(Mesh&& other) = default;
    Mesh& operator=(Mesh&& other) = default;
    //TODO: delete buffers

    Shader& get_shader() { return get_mesh_shader(); }
    void render(const glm::vec3& camera_pos);

private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    int num_indices;
    Material material;
    
    // singleton
    // we use 1 shader for all meshes
    // can't be a static field because it can't be constructed on program initialization
    static Shader& get_mesh_shader() {
        static Shader shader({
            std::pair<std::string, GLuint>("src/openGL/shaders/mesh.vrtx", GL_VERTEX_SHADER),
            std::pair<std::string, GLuint>("src/openGL/shaders/mesh.frgmnt", GL_FRAGMENT_SHADER)
        });
        return shader;
    }
};

#endif
