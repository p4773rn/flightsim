#ifndef CLASS_ARROW_GL
#define CLASS_ARROW_GL

#include <glm/glm.hpp>
#include "../shaders/shader.h"

class Arrow{
public:
	Arrow();
	~Arrow();
	void set_uniforms(const glm::mat4& view, const glm::mat4& projection, float distance);
	void draw(const glm::vec3& origin, const glm::vec3& direction, float scale);
private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	Shader shader;
};
#endif
