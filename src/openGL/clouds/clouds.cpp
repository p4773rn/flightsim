#include "clouds.h"
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>


namespace {
    const float cloudscape_size = 20000;
    const float radius = 10000;
    const float thickness = 200;
}


Clouds::Clouds() :
	shader({{"src/openGL/shaders/clouds.vrtx", GL_VERTEX_SHADER},
			{"src/openGL/shaders/clouds.frgmnt", GL_FRAGMENT_SHADER}}),
	top("assets/clouds/clouds_top.png", false, "clouds top"),
	top_normal("assets/clouds/clouds_top_normal.png", false, "clouds top normal"),
	bottom("assets/clouds/clouds_bottom.png", false, "clouds bottom"),
	bottom_normal("assets/clouds/clouds_bottom_normal.png", false, "clouds bottom normal"),
	tex_coord_offset(0)
{
    pos = glm::vec3(80,100,10);
    std::vector<float> vertices;

    int num_r = 200;
    int num_theta = 200;
    for (int ri = 0; ri < num_r; ++ri) {
        //int num_theta = 1 + (float(ri) / num_r) * (float(ri) / num_r) * 100;
        for (int ti = 0; ti < num_theta; ++ti) {
            float r0 = glm::pow(float(ri)/num_r,4);
            float r1 = glm::pow(float(ri + 1)/num_r,4);
            float t0 = (float(ti)/num_theta) * M_PI * 2;
            float t1 = (float(ti+1)/num_theta) * M_PI * 2;

            float v0x = glm::cos(t0) * r0;
            float v0z = glm::sin(t0) * r0;
            float v1x = glm::cos(t0) * r1;
            float v1z = glm::sin(t0) * r1;
            float v2x = glm::cos(t1) * r1;
            float v2z = glm::sin(t1) * r1;
            float v3x = glm::cos(t1) * r0;
            float v3z = glm::sin(t1) * r0;

            vertices.push_back(v0x);
            vertices.push_back(v0z);
            vertices.push_back(v1x);
            vertices.push_back(v1z);
            vertices.push_back(v2x);
            vertices.push_back(v2z);
            vertices.push_back(v2x);
            vertices.push_back(v2z);
            vertices.push_back(v3x);
            vertices.push_back(v3z);
            vertices.push_back(v0x);
            vertices.push_back(v0z);
        }
    }
    num_vertices = vertices.size();

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


}


Clouds::~Clouds() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}


void Clouds::front_pass(const glm::vec3& camera_pos,
                        const glm::mat4& projection,
                        const glm::mat4& view) {

	
	glBindVertexArray(vao);
	shader.use();

    shader.set("heightmap", 0);
    shader.set("normalmap", 1);
    shader.set("cloudscape_size", cloudscape_size);
    shader.set("radius", radius);
    shader.set("thickness", thickness);
    shader.set("view", view);
    shader.set("projection", projection);
    shader.set("pos", pos + glm::vec3(0,y_offset,0));
  	shader.set("tex_coord_offset", tex_coord_offset);
   

    shader.set("front_or_back", true);
    top.activate(GL_TEXTURE0);
    top_normal.activate(GL_TEXTURE1);
    shader.set("y_flip", 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    
    bottom.activate(GL_TEXTURE0);
    bottom_normal.activate(GL_TEXTURE1);
    shader.set("y_flip", -1.0f);
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	
	glBindVertexArray(0);
}



void Clouds::back_pass(const glm::vec3& camera_pos,
                       const glm::mat4& projection,
                       const glm::mat4& view) {
	glBindVertexArray(vao);
	shader.use();

    shader.set("heightmap", 0);
    shader.set("normalmap", 1);
    shader.set("cloudscape_size", cloudscape_size);
    shader.set("radius", radius);
    shader.set("thickness", thickness);
    shader.set("view", view);
    shader.set("projection", projection);
    shader.set("pos", pos + glm::vec3(0,y_offset,0));
  	shader.set("tex_coord_offset", tex_coord_offset);
    

    shader.set("front_or_back", false);
    top.activate(GL_TEXTURE0);
    top_normal.activate(GL_TEXTURE1);
    shader.set("y_flip", 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    
    bottom.activate(GL_TEXTURE0);
    bottom_normal.activate(GL_TEXTURE1);
    shader.set("y_flip", -1.0f);
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	
	glBindVertexArray(0);
}


bool Clouds::is_in_clouds(const glm::vec3& pos_) const {
    glm::vec3 tex3 = glm::vec3(0.5) + 0.5f * pos_ / cloudscape_size;
    glm::vec2 tex(tex3.x, tex3.z);
    tex += tex_coord_offset;
    float th = this->pos.y + y_offset + thickness * top.sample(tex.x, tex.y).x;
    float bh = this->pos.y + y_offset - thickness * bottom.sample(tex.x, tex.y).x;
    return bh - 2 <= pos_.y && pos_.y <= th + 2; 
}


void Clouds::set_pos_offset(const glm::vec3& offset) {
    tex_coord_offset = glm::vec2(offset.x, offset.z) / cloudscape_size / 2.0f;
    y_offset = -offset.y;
}
