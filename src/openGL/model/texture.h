#ifndef TEXTURE_CLASS
#define TEXTURE_CLASS
#include <string>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <iostream>

class Texture {
private:
    GLuint id = 0;
    sf::Image image;
    void release();
public:
    Texture(const std::string& path,
            bool generateMipmap = true,
            const std::string& t_name = "");
    Texture() { id = 0;};
    Texture(const Texture& t) = delete;
    Texture& operator=(const Texture& t) = delete;
    ~Texture() {release();};
    Texture(Texture&& other) : id(other.id), image(std::move(other.image)), name(std::move(other.name)) { other.id = 0;};
    Texture& operator=(Texture&& t) {
        if (this != &t) {
            std::cout << "Texture& operator=(Texture&& t): " << name << std::endl;
            release();
            std::swap(id, t.id);
            std::swap(image, t.image);
            std::swap(name, t.name);
        }
        return *this;
    };
    void activate(GLenum unit);
    std::string name;

    //glm::uvec2 get_size() const { return glm::uvec2(image.getSize().x, image.getSize().y); }
    //const GLubyte* getPixels() const { return image.getPixelsPtr(); }
    glm::vec4 get_pixel(GLuint px, GLuint py) const;

    // channel: 0-red, 1-green, 2-blue, 3-alpha
    //float get_pixel_channel(GLuint px, GLuint py, int channel) const;

    // like texture() in glsl with linear interpolation
    glm::vec4 sample(float x, float y) const;
    //float sample_channel(float x, float y, int channel) const;
    // sampling 1 channel instead of 4 doesn't seem to improve performance
    
    static void test();

};

#endif
