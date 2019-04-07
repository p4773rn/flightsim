#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <map>
#include <utility>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shaders/shader.h"


//// TODO: remove render dist culling from here
//class Renderable {
//public:
//    virtual Shader& get_shader() = 0;
//    //virtual Shader& get_shadow_shader() = 0;
//
//    virtual void render(const glm::vec3& camera_pos) = 0;
//    virtual float get_max_render_dist() const = 0;
//    
//    virtual void on_renderable_iter_start() {};
//    virtual void on_renderable_iter_stop() {};
//};


class Renderable {
public:
    virtual void geometry_pass(const glm::vec3& camera_pos,
                               const glm::mat4& projection,
                               const glm::mat4& view) = 0;
};


class Renderer {
public:
    Renderer(const glm::ivec2& screen_size);

    void queue_render(Renderable* renderable);

    void render(const glm::vec3& light_dir, const glm::vec3& light_color, 
                const glm::mat4 projection, const glm::mat4& view, 
                const glm::vec3& camera_pos);

private:
    // <shader_id, renderable, model matrix>
//    std::map<GLuint, std::map<Renderable*, std::vector<glm::mat4>>> opaque_renderables;
    std::vector<Renderable*> renderables;

    GLuint g_buffer;
    GLuint g_position;
    GLuint g_normal;
    GLuint g_albedo;
    GLuint g_spec;
    GLuint g_flag;

    GLuint quad_vao;
    GLuint quad_vbo;

    Shader lighting_shader;
};


#endif
