#ifndef MULTIMESH_H
#define MULTIMESH_H

#include "../model/mesh.h"
#include "../renderer.h"
#include "terrain.h"
#include <vector>


class MultiMesh : public Renderable {
public:
    // splat_i: 0,1,2,3 - r,g,b,a
    MultiMesh(Mesh& mesh, Terrain& terrain, int count_per_tile, int dist, int splat_i);

    void update(const glm::vec3 camera_pos);

    
    Shader& get_shader() { return get_multimesh_shader(); }

    void render(const glm::vec3& camera_pos);
    float get_max_render_dist() const { return 1e10; }
    
    void on_renderable_iter_start();
    void on_renderable_iter_stop();
    

private:
    Mesh& mesh;
    Terrain& terrain;
    int count_per_tile;
    int dist;
    int splat_i;

    std::vector<glm::ivec2> tiles;

    static Shader& get_multimesh_shader() {
        static Shader shader({
            std::pair<std::string, GLuint>("src/openGL/shaders/multimesh.vrtx", GL_VERTEX_SHADER),
            std::pair<std::string, GLuint>("src/openGL/shaders/multimesh.frgmnt", GL_FRAGMENT_SHADER)
        });
        return shader;
    }
};



#endif
