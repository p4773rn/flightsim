#include "misc.h"


std::string getParentPath(const std::string& path) {
    size_t slashIndex = path.find_last_of("/");
    return path.substr(0, slashIndex);
}


std::ostream& operator << (std::ostream& stream, const glm::vec3& v) {
    stream << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return stream;
}

std::ostream& operator << (std::ostream& stream, const glm::vec2& v) {
    stream << "[" << v.x << ", " << v.y << "]";
    return stream;
}

