#ifndef MISC_H
#define MISC_H

#include <string>
#include <glm/glm.hpp>
#include <iostream>


std::string getParentPath(const std::string& path);

std::ostream& operator << (std::ostream& stream, const glm::vec3& v);
std::ostream& operator << (std::ostream& stream, const glm::vec2& v);

#endif
