#ifndef TERRAIN_H
#define TERRAIN_H

#include "../renderer.h"
#include <array>
#include <glm/glm.hpp>
#include "../shaders/shader.h"
#include "../model/texture.h"


const unsigned int MAX_NODES = 1000;

// for LOD<1 neighbor might become 4 times larger
const float LOD = 1.f;
const float MIN_NODE_SIZE = 256.0f;
const unsigned int MAX_TEXTURES = 4;


/*

2K x 2K texture with resolution of 10 meters/pixel
can be traversed in 0.5 mins from center to edge

with 100 meters/pixel
in 5 mins 

options:
    generate texture procedurally
    100m/px texture with noise
*/


class Terrain : public Renderable {
public:
    Terrain(const glm::vec3& origin, float size);
    
    Shader& get_shader() { return shader; }
    void render(const glm::vec3& camera_pos);
    
    const glm::vec3& get_origin() const { return origin; }
    void set_origin(const glm::vec3& origin) { this->origin = origin; }
    //const glm::vec2& get_tex_coord_offset() const { return tex_coord_offset; }
    //void set_tex_coord_offset(const glm::vec2& tex_coord_offset) { this->tex_coord_offset = tex_coord_offset; }
private:
    struct Node {
        float tl;//tesselation_left;
        float tr;//tesselation_right;
        float tt;//tesselation_top;
        float tb;//tesselation_bottom;

        Node* parent;
        Node* lt;//left_top;
        Node* lb;//left_bottom;
        Node* rt;//right_top;
        Node* rb;//right_bottom;

        // neighbors of the same or double the size
        Node* nl;//neighbor_left;
        Node* nr;//neighbor_right;
        Node* nt;//neighbor_top;
        Node* nb;//neighbor_bottom;
        
        int depth;
        glm::vec2 origin;
        float size;

        std::string name="r";
    };
    
    Node* construct_tree(const glm::vec3& camera_pos);
    Node* create_node(Node* parent, const glm::vec2& origin, float size);
    bool is_divisible(Node* node, const glm::vec3& camera_pos);
    void divide(Node* node, const glm::vec3& camera_pos);
   
    void calc_tess(Node* node);
    void calc_tess_help(Node* c1, Node* c2, float& c1t, float& c2t, Node** c1n, Node** c2n, Node* n, Node* nc1, Node* nc2);

    void render_tree(Node* node, const glm::vec3& camera_pos);
    void render_node(Node* node, const glm::vec3& camera_pos);


    void load_textures();


    float size;
    glm::vec3 origin;

    Node nodes[MAX_NODES];
    size_t node_count;

    Shader shader;
    GLuint vao, vbo;
    Texture heightmap;
    Texture splatmap;
    Texture lightmap;
    Texture textures[MAX_TEXTURES];
    glm::vec2 tex_coord_offset;
};






#endif
