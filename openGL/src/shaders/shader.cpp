#include <iostream>
#include "shader.h"
#include <sstream>

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

void Shader::use(){
	glUseProgram(Shader::program);
}

