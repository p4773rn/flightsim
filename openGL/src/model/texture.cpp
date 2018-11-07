#include "../shaders/shader.h"

model::Texture(const std::string& path) {
	if (!image.loadFromFile(fname)) {
		std::cout << "Error while loading file in Texture:" << fname << std::endl;
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

void model::Texture::activate(GLenum unit) {
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, id);
}

void model::Texture::release() {
	if (id)
		glDeleteTextures(1, &id);
}