#include "../shaders/shader.h"
#include "texture.h"

Texture::Texture(const std::string& path, bool generateMipmap, const std::string& t_name) : name(t_name) {
	//std::cout << "On texture: " << name << std::endl;
	if (!image.loadFromFile(path)) {
		std::cerr << "Error while loading file in Texture:" << path << std::endl;
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


glm::vec4 Texture::get_pixel(GLuint px, GLuint py) const {
    px %= image.getSize().x;
    py %= image.getSize().y;
   
    int i = 4 * (py * image.getSize().y + px);

    glm::vec4 c;
    c.x = image.getPixelsPtr()[i + 0];
    c.y = image.getPixelsPtr()[i + 1];
    c.z = image.getPixelsPtr()[i + 2];
    c.w = image.getPixelsPtr()[i + 3];
    
    return c/255.0f;
}

//float Texture::get_pixel_channel(GLuint px, GLuint py, int channel) const {
//    px %= image.getSize().x;
//    py %= image.getSize().y;
//   
//    int i = 4 * (py * image.getSize().y + px);
//
//    float c;
//    c = image.getPixelsPtr()[i + channel];
//    
//    return c/255.0f;
//}

glm::vec4 Texture::sample(float x, float y) const {
    float px = fmod(image.getSize().x * x, image.getSize().x) - 0.5f;
    float py = fmod(image.getSize().y * y, image.getSize().y) - 0.5f;
    
    float pix = floor(px);
    float piy = floor(py);
    float p0x = pix;
    float p0y = piy;
    float p1x = pix + 1;
    float p1y = piy + 1;

    glm::vec4 s00 = get_pixel(p0x+0.5, p0y+0.5);
    glm::vec4 s10 = get_pixel(p1x+0.5, p0y+0.5);
    glm::vec4 s01 = get_pixel(p0x+0.5, p1y+0.5);
    glm::vec4 s11 = get_pixel(p1x+0.5, p1y+0.5);
    
    float tx = pix - int(pix);
    float ty = piy - int(piy);

    return mix(mix(s00, s10, (tx)), mix(s01, s11, tx), ty);
}


//float fmix(float a, float b, float t) {
//    return a + t * (b - a);
//}
//
//float Texture::sample_channel(float x, float y, int channel) const {
//    float px = fmod(image.getSize().x * x, image.getSize().x) - 0.5f;
//    float py = fmod(image.getSize().y * y, image.getSize().y) - 0.5f;
//    
//    float pix = floor(px);
//    float piy = floor(py);
//    float p0x = pix;
//    float p0y = piy;
//    float p1x = pix + 1;
//    float p1y = piy + 1;
//
//    float s00 = get_pixel_channel(p0x+0.5, p0y+0.5, channel);
//    float s10 = get_pixel_channel(p1x+0.5, p0y+0.5, channel);
//    float s01 = get_pixel_channel(p0x+0.5, p1y+0.5, channel);
//    float s11 = get_pixel_channel(p1x+0.5, p1y+0.5, channel);
//    
//    float tx = pix - int(pix);
//    float ty = piy - int(piy);
//
//    return fmix(fmix(s00, s10, (tx)), fmix(s01, s11, tx), ty);
//}

void Texture::test() {
    Texture t("assets/test.png");
    sf::Image im;
    im.create(100, 100);
    float scale = 1;

    for (float x = 0.; x < scale; x += 0.001) {
        for (float y = 0.; y < scale; y += 0.001) {
            glm::vec4 s = t.sample(x, y)*255.0f;
            sf::Color col(s.x, s.x, s.x, s.w);
            int px = x/scale * im.getSize().x;
            int py = y/scale * im.getSize().y;
            im.setPixel(px, py, col);
        }
    }

    im.saveToFile("assets/test2.png");
}



