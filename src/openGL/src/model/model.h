#ifndef OBJLOADER
#define OBJLOADER
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include "mesh.h"
#include <memory>
#include "../shaders/shader.h"
#include <map>

class Model{
public:
	Model(const std::string& file_name);
    std::vector<Mesh>& get_meshes() { return meshes; }

private:
	std::string file_name;
	std::vector<Mesh> meshes;

    std::map<std::string, Material> materials;

	void load_obj(const std::string& file_name);
	void load_mtl(const std::string& file_name);
};
#endif
