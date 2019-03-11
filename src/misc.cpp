#include "misc.h"
#include <cmath>

double rad2deg(double rad) {
    return rad / M_PI * 180;
}


double deg2rad(double deg) {
    return deg * M_PI / 180;
}



std::ostream& operator<<(std::ostream& out, const glm::dvec3& v) {
    out << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return out;
}
