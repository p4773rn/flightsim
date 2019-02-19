#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include <utility>
#include <glm/glm.hpp>

class Shader {
private:
    GLuint program;
    std::string read_file(const std::string& path);
    GLuint create_shader(const std::string& path, GLuint shader_type);
    bool check_error(GLuint program, bool isShader, const std::string& error_msg);
public:
    Shader(const std::vector<std::pair<std::string, GLuint>>& paths);
    ~Shader();
    void use();
    GLuint getID() const { return program; }

    void set(const std::string& name, bool p);
    void set(const std::string& name, int p);
    void set(const std::string& name, float p);
    void set(const std::string& name, const glm::vec2& p);
    void set(const std::string& name, const glm::vec3& p);
    void set(const std::string& name, const glm::mat4& p);
    void setv(const std::string& name, int* p, int count);
    void setv(const std::string& name, const glm::ivec2* p, int count);
};

#endif
