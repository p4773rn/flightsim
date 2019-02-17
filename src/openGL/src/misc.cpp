#include "misc.h"
#include <GL/glew.h>


std::string getParentPath(const std::string& path) {
    size_t slashIndex = path.find_last_of("/");
    return path.substr(0, slashIndex);
}


std::ostream& operator << (std::ostream& stream, const glm::vec3& v) {
    stream << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return stream;
}

std::ostream& operator << (std::ostream& stream, const glm::vec2& v) {
    stream << "[" << v.x << ", " << v.y << "]";
    return stream;
}

unsigned int create_quad_vao() {
    unsigned int vao, vbo;

    std::vector<float> vertices = {
        -1, -1, 0,
         1, -1, 0,
        -1,  1, 0,
         1, -1, 0,
         1,  1, 0,
        -1,  1, 0
    };

    glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
	glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);

    return vao;
}

unsigned int get_quad_vao() {
    static unsigned int vao = create_quad_vao();
    return vao;
}
