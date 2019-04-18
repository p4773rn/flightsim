#ifndef CLOUDS_H
#define CLOUDS_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../shaders/shader.h"
#include <memory>
#include "../model/texture.h"


class Clouds {
public:
    Clouds();
    ~Clouds();

    void front_pass(const glm::vec3& camera_pos,
                    const glm::mat4& projection,
                    const glm::mat4& view);
    void back_pass(const glm::vec3& camera_pos,
                   const glm::mat4& projection,
                   const glm::mat4& view);
    
    bool is_in_clouds(const glm::vec3& pos_) const;

    void set_pos_offset(const glm::vec3& offset);
private:

    Shader shader;
    Texture top;
    Texture top_normal;
    Texture bottom;
    Texture bottom_normal;

    GLuint vao;
    GLuint vbo;

    int num_vertices;

    glm::vec3 pos;
    glm::vec2 tex_coord_offset;
    float y_offset = 0;
};


#endif
