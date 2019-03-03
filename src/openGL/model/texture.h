#ifndef TEXTURE_CLASS
#define TEXTURE_CLASS
#include <string>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <iostream>

class Texture {
	private:
		GLuint id = 0;
		void release();
	public:
		Texture(const std::string& path,
				bool generateMipmap = true,
				const std::string& t_name = "");
		Texture() { id = 0;};
		Texture(const Texture& t) = delete;
		Texture& operator=(const Texture& t) = delete;
		~Texture() {release();};
		Texture(Texture&& other) : id(other.id), name(std::move(other.name)) { other.id = 0;};
		Texture& operator=(Texture&& t) {
			if (this != &t) {
				std::cout << "Texture& operator=(Texture&& t): " << name << std::endl;
				release();
				std::swap(id, t.id);
				std::swap(name, t.name);
			}
			return *this;
		};
		void activate(GLenum unit);
		std::string name;
};

#endif
