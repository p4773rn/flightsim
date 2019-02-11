#ifndef TEXTURE_CLASS
#define TEXTURE_CLASS
#include <string>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>

class Texture {
	private:
		GLuint id = 0;
		void release();
	public:
		Texture(const Texture& t) = delete;
		Texture& operator=(const Texture& t) = delete;
		Texture(const std::string& path, bool generateMipmap = true);
		Texture() { id = 0;};
		~Texture() {release();};
		Texture(Texture&& other) : id(other.id) { other.id = 0;};
		Texture& operator=(Texture&& t) {
			if (this != &t) {
				release();
				std::swap(id, t.id);
			}
			return *this;
		};
		void activate(GLenum unit);
};

#endif