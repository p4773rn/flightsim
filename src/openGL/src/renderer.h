#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <utility>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shaders/shader.h"

class Renderable {
public:
    virtual Shader& get_shader() = 0;
    //virtual Shader& get_shadow_shader() = 0;

    virtual void render(const glm::vec3& camera_pos) = 0;
    virtual float get_max_render_dist() const = 0;
};


class Renderer {
public:
    Renderer(const glm::ivec2& screen_size);

    // TODO: terrain doesn't use this model transform lol
    void queue_render(Renderable* renderable, const glm::mat4& model);

    void render(const glm::vec3& light_dir, const glm::vec3& light_color, 
                const glm::mat4 projection, const glm::mat4& view, 
                const glm::vec3& camera_pos);

private:
    // TODO: minimize some function calls by sorting
    std::vector<std::pair<Renderable*, glm::mat4>> opaque_renderables;

    GLuint g_buffer;
    GLuint g_position;
    GLuint g_normal;
    GLuint g_albedo;
    GLuint g_spec;
    GLuint g_flag;

    std::unique_ptr<Shader> lighting_shader;
};


#endif
