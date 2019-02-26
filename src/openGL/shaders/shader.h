#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <initializer_list>

class Shader {
	private:
		GLuint program;
		std::string read_file(const std::string& path);
		GLuint create_shader(const std::string& path, GLuint shader_type);
		bool check_error(GLuint program, bool isShader, const std::string& error_msg);
	public:
		Shader(const std::vector<std::pair<std::string, GLuint>>& paths);
		Shader(std::initializer_list<std::pair<std::string, GLuint>> paths);
		~Shader();
		void use();
		GLuint getID() const { return program; }
};

#endif
