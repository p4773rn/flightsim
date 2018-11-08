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

using std::cout;
using std::endl;

Model::Model(const std::string& fname) : 
    file_name{fname},
    shader({
               std::pair<std::string, GLuint>("shaders/basic.vrtx", GL_VERTEX_SHADER),
               std::pair<std::string, GLuint>("shaders/basic.frgmnt", GL_FRAGMENT_SHADER)
           })
{
    textures.add("assets/models/tree1.png");
    textures.add("assets/models/tree.png");
    load_obj(file_name);
    //load_mtl("assets/models/BGEAR_plane.mtl");
    shader.use();
    textures.setLocation(shader.getID(), "maps");

    for (unsigned int i = 0; i < materials.size(); ++i) {
        std::string prefix = "materials[" + std::to_string(i) + "]";
        const auto& mat = materials[i];
        glUniform3fv(glGetUniformLocation(shader.getID(), (prefix + ".diffuseColor").c_str()),
                     1, glm::value_ptr(mat.diffuseColor));
        glUniform3fv(glGetUniformLocation(shader.getID(), (prefix + ".specularColor").c_str()),
                     1, glm::value_ptr(mat.specularColor));
        glUniform1i(glGetUniformLocation(shader.getID(), (prefix + ".diffuseMapI").c_str()),
                     mat.diffuseMapI);
        glUniform1i(glGetUniformLocation(shader.getID(), (prefix + ".specularMapI").c_str()),
                     mat.specularMapI);
        glUniform1f(glGetUniformLocation(shader.getID(), (prefix + ".shininess").c_str()),
                     mat.shininess);
    }
}

void Model::load_obj(const std::string& file_name) {
    std::string dir = getParentPath(file_name);

	std::vector<glm::vec3> vs;
	std::vector<glm::vec2> vts;
	std::vector<glm::vec3> vns;

	std::vector<Vertex> vertices;
    int materialI = -1;

	//parse OBJ file
	std::ifstream model_file(file_name);
	for(std::string line; getline(model_file, line);) {
		std::istringstream sin(line);
        std::string key;
        sin >> key;
    
        if (key == "mtllib") {
            std::string mtl_file_name;
            sin >> mtl_file_name;
            cout << dir + "/" + mtl_file_name << endl;
            load_mtl(dir + "/" + mtl_file_name);
        }
		else if (key == "v") {
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
        else if (key == "usemtl") {
            std::string materialName;
            sin >> materialName;
            materialI = materialNameIndex[materialName];
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
                v.materialI = materialI;

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
	//    cout << v << endl;
	
	cout << "Loaded obj_vertices: " << vertices.size() << endl;
	only = std::make_unique<Mesh>(vertices);
}

void Model::load_mtl(const std::string& file_name) {
    std::string dir = getParentPath(file_name);
	
	//parse MTL file
	std::ifstream model_file(file_name);
	for(std::string line; getline(model_file, line);) {
		std::istringstream sin(line);
        std::string key;
        sin >> key;

		if (key == "newmtl") {
			std::string name;
			sin >> name;
			materials.push_back(Material());
			materialNameIndex[name] = materials.size() - 1;
		}
		else if (key == "Ns") {
            sin >> materials.back().shininess;
        }
        else if (key == "Kd") {
            Material& mat = materials.back();
            sin >> mat.diffuseColor.x >> mat.diffuseColor.y >> mat.diffuseColor.z;
        }
        else if (key == "Ks") {
            Material& mat = materials.back();
            sin >> mat.specularColor.x >> mat.specularColor.y >> mat.specularColor.z;
        }
        else if (key == "Ks") {
            Material& mat = materials.back();
            sin >> mat.specularColor.x >> mat.specularColor.y >> mat.specularColor.z;
        }
        else if (key == "map_Kd") {
            std::string path;
            sin >> path;
            textures.add(dir + "/" + path);
            Material& mat = materials.back();
            mat.diffuseMapI = textures.size() - 1;
        }
        else if (key == "map_Ks") {
            std::string path;
            sin >> path;
            textures.add(dir + "/" + path);
            Material& mat = materials.back();
            mat.specularMapI = textures.size() - 1;
        }
	}

    
    //for (auto nameIndex : materialNameIndex) {
    //    auto& mat = materials[nameIndex.second];
    //    cout << "Material " << nameIndex.first << endl;
    //    cout << mat.diffuseColor << " " << mat.specularColor << endl;
    //    cout << mat.diffuseMapI << " " << mat.specularMapI << endl;
    //    cout << mat.shininess << endl;
    //}
    cout << "Loaded materials: " << materials.size() << endl;
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

void Model::draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& light){
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"),
                       1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "view"),
                       1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "projection"),
                       1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(glGetUniformLocation(shader.getID(), "lightSource"), 
                       1, glm::value_ptr(light));
	textures.activate();
    only->draw();
}
