#include <iostream>
#include "shader.h"
#include <sstream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::vector<std::pair<std::string, GLuint>>& paths){
	program = glCreateProgram();
	std::vector<GLuint> shaders;
	for (const auto& p : paths) {
		GLuint sh = create_shader(p.first, p.second);
		glAttachShader(program, sh);
		shaders.push_back(sh);
	}
	glLinkProgram(program);
	check_error(program, false, "LINKING::ERROR");

	for (auto sh : shaders)
	    glDeleteShader(sh);
}

Shader::~Shader() {
    glDeleteProgram(program);
}

std::string Shader::read_file(const std::string& path){
	std::ifstream shader_file(path);
	if (!shader_file.is_open())
	    std::cerr << "Can't open " << path << std::endl;
	std::stringstream buffer;
	buffer << shader_file.rdbuf();
	return buffer.str();
}

GLuint Shader::create_shader(const std::string& path, GLuint shader_type){
	std::string text = read_file(path);
	const GLchar* source = text.c_str();
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	check_error(shader, true, "SHADER::COMPILE ERROR");
	
	return shader;
}

bool Shader::check_error(GLuint program, bool isShader,const std::string& error_msg){
	int success;
	char infoLog[512];
	if(isShader){
		glGetShaderiv(program, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(program, 512, NULL, infoLog);
			std::cerr << error_msg << std::endl << infoLog << std::endl;
			return false;
		}
	}
	else{
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if(!success){
			glGetProgramInfoLog(program, 512,NULL, infoLog);
			std::cerr << error_msg << std::endl << infoLog << std::endl;
			return false;
		}
	}

	return true;
}

void Shader::use() {
	glUseProgram(Shader::program);
}



void Shader::set(const std::string& name, bool p) {
    glUniform1i(glGetUniformLocation(program, name.c_str()), p);
}

void Shader::set(const std::string& name, int p) {
    glUniform1i(glGetUniformLocation(program, name.c_str()), p);
}


void Shader::set(const std::string& name, float p) {
    glUniform1f(glGetUniformLocation(program, name.c_str()), p);
}

void Shader::set(const std::string& name, const glm::vec2& p) {
    glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, glm::value_ptr(p));
}

void Shader::set(const std::string& name, const glm::vec3& p) {
    glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, glm::value_ptr(p));
}

void Shader::set(const std::string& name, const glm::mat4& p) {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(p));
}

void Shader::setv(const std::string& name, int* p, int count) {
  	glUniform1iv(glGetUniformLocation(program, name.c_str()), count, p);
}





