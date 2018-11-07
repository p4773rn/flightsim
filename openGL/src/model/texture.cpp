#include "../shaders/shader.h"
#include "texture.h"

Texture::Texture(const std::string& path) {
	sf::Image image;
	if (!image.loadFromFile(path)) {
		std::cout << "Error while loading file in Texture:" << path << std::endl;
		return;
	}
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				image.getSize().x, image.getSize().y,
				0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::activate(GLenum unit) {
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::release() {
	if (id)
		glDeleteTextures(1, &id);
}