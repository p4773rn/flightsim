#include "model.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include <SFML/Graphics/Image.hpp>
#include "../misc.h"
#include "ac3d_loader.h"
#include <queue>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

using std::cout;
using std::endl;

Model::Model(const std::string& fname) : 
    file_name{fname}
{ 
    //load_obj(file_name);
    AC3D_loader ac_loader(fname);
	//exit(1);
	std::cout << "Model " << fname << " was loaded...\n";
	objects.swap(ac_loader.objects);
	textures.swap(ac_loader.textures);
	materials.swap(ac_loader.materials);
	std::cout << "Number of objects:" << objects.size() << std::endl;
	std::cout << "Successfully copied objects, textures, materials to model\n";
}

void Model::draw(const Shader& shader){
    shader.use();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model *= glm::yawPitchRoll(rotation.x, rotation.y, rotation.z);
    model *= glm::yawPitchRoll(default_rotation.x, default_rotation.y, default_rotation.z);
    draw_rec(shader, model, 0);
}
//glm::yawPitchRoll(yaw, pitch, roll)
void Model::draw_rec(const Shader& shader, const glm::mat4& model, int index) {
	//17635 - vertices only
	//5531 - vertices while using indices
	//exit(1);
	glm::mat4 TR(1.0f);
	TR = glm::translate(TR, objects[index].position);
	TR *= glm::yawPitchRoll(objects[index].yaw_roll_pitch.x,
							objects[index].yaw_roll_pitch.y,
							objects[index].yaw_roll_pitch.z);
	if (!objects[index].indices.empty()){
		bool has_texture = objects[index].texture_id > -1;
		glUniform1i(shader.get_loc("is_texture"), has_texture);
		if (has_texture) {
			textures[objects[index].texture_id].activate(GL_TEXTURE0);
			glUniform1i(shader.get_loc("texture0"), 0);
		}
		if (objects[index].material_id > -1) {
			auto& m = materials[objects[index].material_id];
			//std::cout << m.ambient.x << " " << m.ambient.y << " " << m.ambient.z << std::endl;
			glUniform3fv(shader.get_loc("diffuse_color"), 1, glm::value_ptr(m.rgb));
			glUniform3fv(shader.get_loc("ambient"), 1, glm::value_ptr(m.ambient));
			glUniform3fv(shader.get_loc("emission"), 1, glm::value_ptr(m.emission));
			glUniform3fv(shader.get_loc("specular"), 1, glm::value_ptr(m.specular));
			glUniform1i(shader.get_loc("shininess"), m.shininess);
			glUniform1f(shader.get_loc("alpha"), m.transparency);
		}

		glUniformMatrix4fv(shader.get_loc("model"), 1, GL_FALSE, glm::value_ptr(model * TR));
		glBindVertexArray(objects[index].VAO);
		glDrawElements(GL_TRIANGLES, objects[index].indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	for (int i : objects[index].children)
		draw_rec(shader, model * TR, i);
}
