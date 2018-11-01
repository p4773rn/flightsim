#ifndef OBJLOADER
#define OBJLOADER
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include "mesh.h"
#include <memory>

class Model{
private:
	std::string file_name;
	std::unique_ptr<Mesh> only;
	GLuint texture;
public:
	Model(const std::string& file_name, const std::string& texturepath);
	glm::mat4 scale();
	void load_obj(const std::string& file_name);
    void load_texture(const std::string& texturepath);
	void draw();
};
#endif
