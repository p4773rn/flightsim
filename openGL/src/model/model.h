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

struct Material {
    glm::vec3 diffuseColor = {0.8, 0.8, 0.8};
    glm::vec3 specularColor = {1.0, 1.0, 1.0};

    int diffuseMapI = -1;
    int specularMapI = -1;

    float shininess = 32;
};


class Model{
private:
	std::string file_name;
	std::unique_ptr<Mesh> only;
    
    Shader shader;
    TextureArray textures;
	std::vector<Material> materials;
    std::map<std::string, int> materialNameIndex;

	void load_obj(const std::string& file_name);
	void load_mtl(const std::string& file_name);
public:
	Model(const std::string& file_name);
	glm::mat4 scale();
	void draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& light);
};
#endif
