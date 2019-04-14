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
#include "ac3d_loader.h"
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


void Model::draw_rec(Shader& shader, const glm::mat4& model, int index,
                     const glm::mat4& projection,
                     const glm::mat4& view) {
	//17635 - vertices only
	//5531 - vertices while using indices
	//exit(1);
	glm::mat4 TR(1.0f);
	TR = glm::translate(TR, objects[index].position);
	
	TR *= (glm::mat4)glm::mat4_cast(objects[index].orientation);
	if (!objects[index].indices.empty()){
		bool has_texture = objects[index].texture_id > -1;
		shader.set("use_texture", has_texture);
		if (has_texture) {
			textures[objects[index].texture_id].activate(GL_TEXTURE0);
			shader.set("texture0", 0);
		}
		if (objects[index].material_id > -1) {
			auto& m = materials[objects[index].material_id];
			shader.set("diffuse_color", m.rgb);
			// ac3d's shininess doesn't correspond shininess in our lighting model
			shader.set("shininess", 0.1f);//m.shininess);
		}
        
	    shader.set("model", model * TR);
	    shader.set("view", view);
	    shader.set("projection", projection);
		glBindVertexArray(objects[index].VAO);
		glDrawElements(GL_TRIANGLES, objects[index].indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	for (int i : objects[index].children)
		draw_rec(shader, model * TR, i, projection, view);
}


void Model::geometry_pass(const glm::vec3& camera_pos,
                          const glm::mat4& projection,
                          const glm::mat4& view) {
    Shader& shader = get_shader(); 
    shader.use();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model *= (glm::mat4)glm::mat4_cast(orientation); //glm::yawPitchRoll(rotation.x, rotation.y, rotation.z);
    model *= glm::yawPitchRoll(default_rotation.x, default_rotation.y, default_rotation.z);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    draw_rec(shader, model, 0, projection, view);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
