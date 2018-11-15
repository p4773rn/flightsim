#ifndef MISC_H
#define MISC_H

#include <string>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

std::string getParentPath(const std::string& path);

std::ostream& operator << (std::ostream& stream, const glm::vec3& v);
std::ostream& operator << (std::ostream& stream, const glm::vec2& v);

template<typename T>
std::ostream& operator << (std::ostream& stream, const std::vector<T>& v) {
    std::cout << '[';
    for (const T& t : v)
        std::cout << t << ", ";
    std::cout << ']';
    return stream;
}

#endif
