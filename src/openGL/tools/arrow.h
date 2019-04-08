#ifndef CLASS_ARROW_GL
#define CLASS_ARROW_GL

#include <glm/glm.hpp>
#include "../shaders/shader.h"
#include "../renderer.h"

class Arrow : public Renderable{
public:
	Arrow();
	~Arrow();
	void set_arrows(const std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& debug_arrows, 
	                float camera_distance);
	void geometry_pass(const glm::vec3& camera_pos,
                       const glm::mat4& projection,
                       const glm::mat4& view);
private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	Shader shader;

	std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> debug_arrows;
	float camera_distance;
};
#endif
