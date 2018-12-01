#ifndef OBJLOADER
#define OBJLOADER
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include "mesh.h"
#include <memory>
#include "texturearray.h"
#include "../shaders/shader.h"
#include <map>

class Model{
private:
	std::string file_name;
	std::vector<Mesh> meshes;

    Shader shader;
    std::map<std::string, Material> materials;

	void load_obj(const std::string& file_name);
	void load_mtl(const std::string& file_name);
public:
	Model(const std::string& file_name);
	void draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, 
              const glm::vec3& cameraPos, const glm::vec3& light);
};
#endif
