#include <iostream>
#include "shader.h"

Shader::Shader(const std::vector<std::pair<std::string, GLuint>>& paths){
	program = glCreateProgram();
	for (const auto& p : paths) {
		GLuint sh = create_program(p.first, p.second);
		glAttachShader(program, sh);
	}
	glLinkProgram(program);
	check_error(program, false, "LINKING::ERROR");
}

std::string Shader::read_file(const std::string& path){
	std::string text;
	std::ifstream shader_file(path);
	for(std::string line; getline(shader_file, line);){
		text += line;
		text += "\n";
	}
	return text;
}

GLuint Shader::create_program(const std::string& path, GLuint shader_type){
	std::string text = read_file(path);
	GLuint shader;
	const GLchar* source = text.c_str();
	shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	check_error(shader, true, "SHADER::COMPILE ERROR");
	source = NULL;
	//std::cout << text << std::endl;
	return shader;
}

bool Shader::check_error(GLuint program, bool isShader,const std::string& error_msg){
	int success;
	char infoLog[512];
	if(isShader){
		glGetShaderiv(program, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(program, 512, NULL, infoLog);
			std::cout << error_msg << std::endl << infoLog << std::endl;
			return false;
		}
	}
	else{
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if(!success){
			glGetProgramInfoLog(program, 512,NULL, infoLog);
			std::cout << error_msg << std::endl << infoLog << std::endl;
			return false;
		}
	}

	return true;
}

void Shader::use(){
	glUseProgram(Shader::program);
}

GLuint Shader::getID(){
	return program;
}
