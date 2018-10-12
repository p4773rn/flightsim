#include "model.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>

Model::Model(const std::string& fname) : file_name{fname}{
  load_obj(file_name);
}

void Model::load_obj(const std::string& file_name) {
	std::vector<glm::vec3> obj_vertices;
	std::vector<glm::vec3> obj_normals;
	std::vector< std::pair<GLuint, GLuint> > indices;
	std::vector<Vertex> vertices;

	//parse OBJ file
	std::ifstream model_file(file_name);
	for(std::string line; getline(model_file, line);) {
		// obj_vertices
		if(line.substr(0, 2) == "v ") {
			glm::vec3 t_vert;
			std::istringstream s(line.substr(2));
			s >> t_vert.x; s >> t_vert.y; s >> t_vert.z;
			obj_vertices.push_back(t_vert);
		}
		// normals
		else if (line.substr(0, 3) == "vn ") {
			glm::vec3 t_norm;
			std::istringstream s(line.substr(3));
			s >> t_norm.x; s >> t_norm.y; s >> t_norm.z;
			obj_normals.push_back(t_norm);
		}
		// indices
		else if(line.substr(0, 2) == "f ") {
			unsigned int i = 0;
			GLuint vertexIndex, uvIndex, normalIndex;
			std::istringstream ss(line.substr(2));
			//std::cout << line.substr(2) << std::endl;
			while(!ss.eof()){
				i++;
				ss >> vertexIndex >> std::ws;
				if (ss.peek() == '/') {
					ss.get();
					if (ss.peek() == '/') {
						ss.get();
						ss >> normalIndex >> std::ws;
					} else {
						ss >> uvIndex >> std::ws;
						if (ss.peek() == '/') {
							ss.get();
							ss >> normalIndex >> std::ws;
						}
					}
				}
				//std::cout << vertexIndex << '/' << uvIndex << '/' << normalIndex << std::endl;
				if (i <= 3){
					indices.push_back(std::make_pair(vertexIndex-1, normalIndex-1));
					//std::cout << vertexIndex-1 << ' ' << normalIndex - 1 << std::endl;
				}
				else{
					auto t1 = indices[indices.size() - i + 1];
					auto t2 = indices[indices.size() - 1];
					indices.push_back(t1);
					indices.push_back(std::make_pair(vertexIndex-1, normalIndex-1));
					indices.push_back(t2);
				}
			}
		}
	}

	for (const auto& indice_pair: indices) {
		/*std::cout << obj_vertices[indice_pair.first].x << ' '
				  << obj_vertices[indice_pair.first].y<< ' '
				  << obj_vertices[indice_pair.first].z<< ' '

				  << obj_normals[indice_pair.second].x << ' '
				  << obj_normals[indice_pair.second].y << ' '
				  << obj_normals[indice_pair.second].z << ' '
				  << std::endl;*/
		Vertex v = { obj_vertices[indice_pair.first], obj_normals[indice_pair.second] };
		/*std::cout << v.position.x << ' ' << v.position.y << ' ' << v.position.z << ' '
				  << v.normal.x << ' ' << v.normal.y << ' ' << v.normal.z
				  << std::endl << "-----------" << std::endl;*/
		vertices.push_back(v);
	}

	std::cout << "Loaded obj_vertices: " << vertices.size() << std::endl;
	only = std::make_unique<Mesh>(Mesh(vertices));
}

glm::mat4 Model::scale(){
  glm::mat4 scale = glm::mat4(1.0f);
	float max = 0.0f;
	for (unsigned int i = 0; i < only->vertices.size(); i++){
		float mofv = std::max(only->vertices[i].position.x,
                    std::max(only->vertices[i].position.y, only->vertices[i].position.z) );
		if (mofv > max)
			max = mofv;
	}
	return glm::scale(scale, glm::vec3(1.0f/max, 1.0f/max, 1.0f/max));
}

void Model::draw(){
	only->draw();
}