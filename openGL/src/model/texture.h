#ifndef TEXTURE_CLASS
#define TEXTURE_CLASS
#include <string>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>

class model::Texture {
	private:
		sf::Image image;
		GLuint id;
	public:
		Texture(const Texture& t) = delete;
		Texture& operator=(const Texture& t) = delete;
		Texture(const std::string& path);
		~Texture() {release();};
		Texture& operator=(Texture&& t) {
			if (this != &t) {
				release();
				std::swap(image, t.image);
				std::swap(id, t.id);
			}
		};
		void activate(GLenum unit);
		void release();
}

#endif