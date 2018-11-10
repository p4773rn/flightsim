#ifndef TEXTUREARRAY_H
#define TEXTUREARRAY_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class TextureArray {
public:
    void add(const std::string& path);
    void setLocation(GLuint shaderID, const std::string& varname);
    void activate();
    size_t size() const { return ids.size(); }
    
    TextureArray() {};
    TextureArray(const TextureArray& t) = delete;
    TextureArray& operator=(const TextureArray& t) = delete;
    ~TextureArray();
private:
    std::vector<GLuint> ids;
};


#endif
