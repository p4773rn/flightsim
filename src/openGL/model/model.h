#ifndef OBJLOADER
#define OBJLOADER
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include "texture.h"
#include "../shaders/shader.h"
#include "../renderer.h"

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
	glm::dquat orientation;
	int texture_id = -1;
	int material_id = -1;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	std::vector<int> children;
};



class Model : public Renderable{
private:
	std::string file_name;
	std::vector<Object> objects;
	std::vector<Texture> textures;
	std::vector<Material> materials;
	void draw_rec(Shader& shader, const glm::mat4& model, int i,
                  const glm::mat4& projection,
                  const glm::mat4& view);
	glm::vec3 position;
	glm::dquat orientation;
	glm::vec3 default_rotation;
    //Shader shader;

    static Shader& get_shader() {
        static Shader shader({
            std::pair<std::string, GLuint>("src/openGL/shaders/model.vrtx", GL_VERTEX_SHADER),
            std::pair<std::string, GLuint>("src/openGL/shaders/model.frgmnt", GL_FRAGMENT_SHADER)
        });
        return shader;
    }
public:
	Model(const std::string& file_name);
	void geometry_pass(const glm::vec3& camera_pos,
                       const glm::mat4& projection,
                       const glm::mat4& view);
	void set_position(const glm::vec3& pos){
		position = pos;
	};
	void set_orientation(const glm::dquat& _orientation) {
		orientation = _orientation;
	};
	void set_default_rotation(const glm::vec3& _yaw_roll_pitch) {
		default_rotation = _yaw_roll_pitch;
	};
};
#endif
