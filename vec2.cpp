#include "vec2.h"

Vec2 operator * (double s, const Vec2& v) {
    return {v.getX() * s, v.getY() * s};
}

Vec2 operator * (const Vec2& v, double s) {
    return {v.getX() * s, v.getY() * s};
}

std::ostream& operator << (std::ostream& stream, const Vec2& v) {
    stream << "[" << v.getX() << ", " << v.getY() << "]";
    return stream;
}
