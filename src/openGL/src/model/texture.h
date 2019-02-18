#ifndef TEXTURE_CLASS
#define TEXTURE_CLASS
#include <string>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>

class Texture {
private:
    GLuint id = 0;
    sf::Image image;
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
            std::swap(image, t.image);
        }
        return *this;
    };
    void activate(GLenum unit);

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
