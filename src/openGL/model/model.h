#ifndef OBJLOADER
#define OBJLOADER
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include "texture.h"
#include "../shaders/shader.h"

struct Material {
	std::string name;
	glm::vec3 rgb;
	glm::vec3 ambient;
	glm::vec3 emission;
	glm::vec3 specular;
	int shininess;
	float transparency;
};

struct Vertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct Object {
	std::string name;
	std::string type;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	glm::vec3 position;
	glm::vec3 yaw_roll_pitch;
	int texture_id = -1;
	int material_id = -1;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	std::vector<int> children;
};



class Model{
private:
	std::string file_name;
	std::vector<Object> objects;
	std::vector<Texture> textures;
	std::vector<Material> materials;
	void draw_rec(const Shader& shader, const glm::mat4& model, int i);
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 default_rotation;
    //Shader shader;
public:
	Model(const std::string& file_name);
	void draw(const Shader& shader);
	void set_position(const glm::vec3& pos){
		position = pos;
	};
	void set_rotation(const glm::vec3& _yaw_roll_pitch) {
		rotation = _yaw_roll_pitch;
	};
	void set_default_rotation(const glm::vec3& _yaw_roll_pitch) {
		default_rotation = _yaw_roll_pitch;
	};
};
#endif
