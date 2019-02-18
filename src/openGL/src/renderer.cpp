#include "renderer.h"
#include <iostream>
#include "misc.h"


/*
Deferred shading
-----------------------------------------------
Large parts of code were copied and modified from https://learnopengl.com/Advanced-Lighting/Deferred-Shading
-----------------------------------------------
All code samples, unless explicitly stated otherwise, are licensed under the terms of the CC BY-NC 4.0 license as published by Creative Commons, either version 4 of the License, or (at your option) any later version.

See https://learnopengl.com/About for more information.
-----------------------------------------------
Author: Joey de Vries
Twitter: https://twitter.com/JoeyDeVriez
License: https://github.com/JoeyDeVries/LearnOpenGL/blob/master/LICENSE.md
*/





Renderer::Renderer(const glm::ivec2& screen_size) {
    // configure g-buffer framebuffer
    glGenFramebuffers(1, &g_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
    // position color buffer
    glGenTextures(1, &g_position);
    glBindTexture(GL_TEXTURE_2D, g_position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screen_size.x, screen_size.y, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_position, 0);
    // normal color buffer
    glGenTextures(1, &g_normal);
    glBindTexture(GL_TEXTURE_2D, g_normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screen_size.x, screen_size.y, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_normal, 0);
    // color
    glGenTextures(1, &g_albedo);
    glBindTexture(GL_TEXTURE_2D, g_albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_size.x, screen_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_albedo, 0);
    // specular (shininess, specular_strength)
    glGenTextures(1, &g_spec);
    glBindTexture(GL_TEXTURE_2D, g_spec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, screen_size.x, screen_size.y, 0, GL_RG, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, g_spec, 0);
    // flags (unused, unused, unused, unused, unused, unused, unused, drawn)
    glGenTextures(1, &g_flag);
    glBindTexture(GL_TEXTURE_2D, g_flag);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8I, screen_size.x, screen_size.y, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, g_flag, 0);
    
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
    glDrawBuffers(5, attachments);
    // create and attach depth buffer (renderbuffer)
    unsigned int rbo_depth;
    glGenRenderbuffers(1, &rbo_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_size.x, screen_size.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    std::vector<std::pair<std::string, GLuint>> paths = {
        std::pair<std::string, GLuint>("src/openGL/shaders/lighting.vrtx", GL_VERTEX_SHADER),
        std::pair<std::string, GLuint>("src/openGL/shaders/lighting.frgmnt", GL_FRAGMENT_SHADER)
    };
    lighting_shader = std::make_unique<Shader>(paths);
}


void Renderer::queue_render(Renderable* renderable, const glm::mat4& model) {
    opaque_renderables[renderable->get_shader().getID()][renderable]
        .push_back(model);
}


void Renderer::render(const glm::vec3& light_dir, const glm::vec3& light_color, 
                      const glm::mat4 projection, const glm::mat4& view, 
                      const glm::vec3& camera_pos) 
{
    // geometry pass
    //-----------------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& srm : opaque_renderables) {
        auto& rms = srm.second;
        
        Shader& shader = rms.begin()->first->get_shader();
        shader.use();
        shader.set("projection", projection);
        shader.set("view", view);
        
        for (auto& rm : rms) {
            auto& models = rm.second;
            Renderable *r = rm.first;
            r->on_renderable_iter_start();

            for (auto& model : models) {
                glm::vec3 pos = glm::vec3(model * glm::vec4(0,0,0,1));
                if (glm::distance(pos, camera_pos) > r->get_max_render_dist())
                    continue;

                shader.set("model", model);
                
                r->render(camera_pos);
            }
            models.clear();
            r->on_renderable_iter_stop();
        }
    }
        

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //opaque_renderables.clear();
   

    // lighting pass
    //-----------------------------------
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_position);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_normal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_albedo);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, g_spec);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, g_flag);

    lighting_shader->use();
    lighting_shader->set("gPosition", 0);
    lighting_shader->set("gNormal", 1);
    lighting_shader->set("gAlbedo", 2);
    lighting_shader->set("gSpec", 3);
    lighting_shader->set("gFlag", 4);
    lighting_shader->set("view", view);
    lighting_shader->set("light_dir", light_dir);
    lighting_shader->set("light_color", light_color);
    lighting_shader->set("camera_pos", camera_pos);
    
    glBindVertexArray(get_quad_vao());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //glBindVertexArray(0);
}
