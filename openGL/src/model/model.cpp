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


Model::Model(const std::string& fname, const std::string& texturepath) : file_name{fname}{
  load_obj(file_name);
  load_texture(texturepath);
}

void Model::load_obj(const std::string& file_name) {
	std::vector<glm::vec3> vs;
	std::vector<glm::vec2> vts;
	std::vector<glm::vec3> vns;

	std::vector<Vertex> vertices;

	//parse OBJ file
	std::ifstream model_file(file_name);
	for(std::string line; getline(model_file, line);) {
		std::istringstream sin(line);
        std::string key;
        sin >> key;

		if (key == "v") {
			glm::vec3 v;
			sin >> v.x >> v.y >> v.z;
			vs.push_back(v);
		}
		else if (key == "vt") {
            glm::vec2 vt;
            sin >> vt.x >> vt.y;
            vts.push_back(vt);
        }
        else if (key == "vn") {
            glm::vec3 vn;
            sin >> vn.x >> vn.y >> vn.z;
            vns.push_back(vn);
        }
        else if (key == "f") {
            for (int i = 0; !sin.eof(); ++i) {
                int vi=0, vti=0, vni=0; // 0 is not a valid index and indicates that index was not initialized
                sin >> vi;
                if (sin.peek() == '/') {
                    sin.get();
                    if (sin.peek() == '/') {
                        sin.get();
                        sin >> vni;
                    } else {
                        sin >> vti;
                        if (sin.peek() == '/') {
                            sin.get();
                            sin >> vni;
                        }
                    }
                }
                sin >> std::ws; // there might be '\r' after the last vertex in a face
                
                Vertex v;
                v.position = vs[vi-1];
                if (vti != 0)
                    v.texcoords = vts[vti-1];
                if (vni != 0)
                    v.normal = vns[vni-1];

                if (i < 3) {
                    vertices.push_back(v);
                } else {
                    Vertex v1 = vertices[vertices.size() - i];
                    Vertex v2 = vertices[vertices.size() - 1];
                    vertices.push_back(v1);
                    vertices.push_back(v2);
                    vertices.push_back(v);
                }
            }
        }
	}

	//for (auto& v : vertices)
	//    std::cout << v.position.x << ' ' << v.position.y << ' ' << v.position.z << ' '
	//			  << v.normal.x << ' ' << v.normal.y << ' ' << v.normal.z
	//			  << v.texcoords.x << ' ' << v.texcoords.y
	//			  << std::endl << "-----------" << std::endl;

	std::cout << "Loaded obj_vertices: " << vertices.size() << std::endl;
	only = std::make_unique<Mesh>(vertices);
}

void Model:: load_texture(const std::string& texturepath) {
    sf::Image textureimage;
    if (!textureimage.loadFromFile(texturepath)) {
        std::cerr << "Error loading texture: " << texturepath << std::endl;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				textureimage.getSize().x, textureimage.getSize().y,
				0, GL_RGBA, GL_UNSIGNED_BYTE, textureimage.getPixelsPtr());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



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
    glBindTexture(GL_TEXTURE_2D, texture);
	only->draw();
}
