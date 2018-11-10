#include "texturearray.h"
#include <SFML/Graphics/Image.hpp>
#include <iostream>

void TextureArray::add(const std::string& path) {
    sf::Image image;
    if (!image.loadFromFile(path)) {
		std::cerr << "Error while loading file in TextureArray:" << path << std::endl;
		return;
	}
	
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				image.getSize().x, image.getSize().y,
				0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ids.push_back(id);
}


void TextureArray::setLocation(GLuint shaderId, const std::string& varname) {
    if (!ids.empty()) {
        GLint ids_[ids.size()];
        for (unsigned int i = 0; i < ids.size(); ++i)
            ids_[i] = i;

        glUniform1iv(glGetUniformLocation(shaderId, varname.c_str()), ids.size(), ids_);
    }
}


void TextureArray::activate() {
    for(unsigned int i = 0; i < ids.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
	    glBindTexture(GL_TEXTURE_2D, ids[i]);
    }
}

TextureArray::~TextureArray() {
    if (!ids.empty())
        glDeleteTextures(ids.size(), &ids[0]);
}
