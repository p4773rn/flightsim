#include "../shaders/shader.h"
#include "texture.h"

Texture::Texture(const std::string& path, bool generateMipmap, const std::string& t_name) : name(t_name) {
	sf::Image image;
	//std::cout << "On texture: " << name << std::endl;
	if (!image.loadFromFile(path)) {
		std::cout << "Error while loading file in Texture:" << path << std::endl;
		return;
	}
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				image.getSize().x, image.getSize().y,
				0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	if (generateMipmap) glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, generateMipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
}

void Texture::activate(GLenum unit) {
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::release() {
	if (id){	
		std::cout << "Texture::release() call at " << name << std::endl;
		glDeleteTextures(1, &id);
	}
}
