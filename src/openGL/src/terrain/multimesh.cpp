#include "multimesh.h"
#include <cassert>

namespace {
    const int TILE_DIM = 10; // TILE_DIM x TILE_DIM grid
    const int TILE_SIZE = 100; // every tile is TILE_SIZE x TILE_SIZE
}


MultiMesh::MultiMesh(Mesh& mesh, Terrain& terrain, int count_per_tile, int dist, int splat_i) :
    mesh{mesh},
    terrain{terrain},
    count_per_tile{count_per_tile},
    dist{dist},
    splat_i{splat_i}
{
    assert(0 <= splat_i && splat_i <= 3);
}

void MultiMesh::update(const glm::vec3 camera_pos) {    
    tiles.clear();
    for (int i = 0; i < TILE_DIM; ++i) {
        for (int j = 0; j < TILE_DIM; ++j) {
            glm::vec3 pos(camera_pos.x + (i - TILE_DIM/2) * TILE_SIZE,
                          0,
                          camera_pos.z + (j - TILE_DIM/2) * TILE_SIZE);
            pos -= glm::mod(pos, float(TILE_SIZE));
            
            glm::vec2 pos2(pos.x, pos.z);

            pos.y = terrain.get_height(pos2);

            if (glm::distance(camera_pos, pos) < dist) {
                glm::vec2 tex_coord = terrain.get_tex_coord(pos2);
                glm::vec4 splat = terrain.get_splatmap().sample(tex_coord.x, tex_coord.y);
               
                
                float v;
                switch(splat_i) {
                case 0:
                    v = splat.x;
                    break;
                case 1:
                    v = splat.y;
                    break;
                case 2:
                    v = splat.z;
                    break;
                case 3:
                    v = splat.w;
                    break;
                }

                if (v > 0.8)
                    tiles.push_back(glm::ivec2(pos.x, pos.z));
            }
        }
    }
}

void MultiMesh::render(const glm::vec3& camera_pos) {
    int count = tiles.size()  * count_per_tile;
    glDrawElementsInstanced(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, 0, count);

}

void MultiMesh::on_renderable_iter_start() {
    Shader& shader = get_shader();
    const Material& material = mesh.material;
    GLuint vao = mesh.vao;


    shader.set("count_per_tile", count_per_tile);
    shader.setv("tiles", &(tiles[0]), tiles.size());
    shader.set("heightmap", 0);
    terrain.get_heightmap().activate(GL_TEXTURE0);
    shader.set("terrain_size", terrain.get_size());

    bool useTexture = material.texture != nullptr;
    shader.set("use_texture", useTexture);

    if (useTexture) {
        material.texture->activate(GL_TEXTURE0);
    } else {
        shader.set("diffuse_color", material.diffuseColor);
    }

    shader.set("shininess", material.shininess);

    glBindVertexArray(vao);
}

void MultiMesh::on_renderable_iter_stop() {
    glBindVertexArray(0);
}
