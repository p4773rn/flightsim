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
    shader({{"src/openGL/shaders/basic.vrtx", GL_VERTEX_SHADER},
            {"src/openGL/shaders/basic.frgmnt", GL_FRAGMENT_SHADER}
           })
{ 
    load_obj(file_name);
	std::cout << "Model " << fname << " was created...\n";
}

void Model::load_obj(const std::string& file_name) {
    std::string dir = getParentPath(file_name);

	std::vector<glm::vec3> vs;
	std::vector<glm::vec2> vts;
	std::vector<glm::vec3> vns;

	std::map<std::string, std::vector<Vertex>> meshData;
    std::string materialName;

	//parse OBJ file
	std::ifstream model_file(file_name);
	if (!model_file.is_open())
	    std::cerr << "Can't open " << file_name << std::endl;

	for(std::string line; getline(model_file, line);) {
		std::istringstream sin(line);
        std::string key;
        sin >> key;
    
        if (key == "mtllib") {
            std::string mtl_file_name;
            sin >> mtl_file_name;
            cout << dir + "/" + mtl_file_name << endl;
            load_mtl(dir + "/" + mtl_file_name);
        } else if (key == "v") {
			glm::vec3 v;
			sin >> v.x >> v.y >> v.z;
			vs.push_back(v);
		} else if (key == "vt") {
            glm::vec2 vt;
            sin >> vt.x >> vt.y;
            vts.push_back(vt);
        } else if (key == "vn") {
            glm::vec3 vn;
            sin >> vn.x >> vn.y >> vn.z;
            vns.push_back(vn);
        } else if (key == "usemtl") {
            sin >> materialName;
        } else if (key == "f") {
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

                auto& vertices = meshData[materialName];
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
	
    for (auto& mv : meshData) {
        auto& materialName = mv.first;
        auto& vertices = mv.second;
        meshes.push_back(Mesh(vertices, std::move(materials[materialName])));
    }
    materials.clear();
}

void Model::load_mtl(const std::string& file_name) {
    std::string dir = getParentPath(file_name);

    std::string materialName;

	//parse MTL file
	std::ifstream model_file(file_name);
	for(std::string line; getline(model_file, line);) {
		std::istringstream sin(line);
        std::string key;
        sin >> key;

		if (key == "newmtl") {
			sin >> materialName;
		} else if (key == "Ns") {
            sin >> materials[materialName].shininess;
        } else if (key == "Kd") {
            Material& mat = materials[materialName];
            sin >> mat.diffuseColor.x >> mat.diffuseColor.y >> mat.diffuseColor.z;
        } else if (key == "Ks") {
            //Material& mat = materials[materialName];
            //sin >> mat.specularColor.x >> mat.specularColor.y >> mat.specularColor.z;
        } else if (key == "map_Kd") {
            std::string path;
            sin >> path;
            Material& mat = materials[materialName];
            mat.texture = std::make_unique<Texture>(dir + "/" + path);
        } else if (key == "map_Ks") {
            //std::string path;
            //sin >> path;
            //textures.add(dir + "/" + path);
            //Material& mat = materials[materialName];
            //mat.specularMapI = textures.size() - 1;
        } else if (key == "d") {
            Material& mat = materials[materialName];
            sin >> mat.alpha;
        } else if (key == "Tr") {
            float tr;
            sin >> tr;
            Material& mat = materials[materialName];
            mat.alpha = 1 - tr;
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

void Model::draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, 
                 const glm::vec3& cameraPos, const glm::vec3& light){
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"),
                       1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "view"),
                       1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "projection"),
                       1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(glGetUniformLocation(shader.getID(), "lightSource"), 
                       1, glm::value_ptr(light));
    
    for (auto& mesh : meshes) {
        if (mesh.transparent()) {
            mesh.sortByZ(model, cameraPos);
        }
        mesh.draw(shader);
    }
}
