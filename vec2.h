#ifndef VEC2_H
#define VEC2_H

#include <cmath>

class Vec2 {
public:
    Vec2(double x, double y) : x(x), y(y)
        {}

    Vec2() : x(0), y(0)
        {}

    double getX() const {return x;}
    double getY() const {return y;}

    double lengthSquared() const {
        return x*x + y*y;
    }

    double length() const {
        return sqrt(lengthSquared());
    }

    Vec2 tangent() const {
        return {-y, x};
    }

    Vec2 normalized() const {
        return (*this) / length();
    }

    void setX(double x) {this->x = x;}
    void setY(double y) {this->y = y;}

    void operator+=(const Vec2& v) {
        x += v.x;
        y += v.y;
    }
    Vec2 operator + ( const Vec2& v ) const {
        return {x + v.getX(), y + v.getY()};
    }
    Vec2 operator * ( double d ) const {
        return {x * d, y * d};
    }
    Vec2 operator / ( double d ) const {
        return {x / d, y / d};
    }

private:
    double x;
    double y;
};



#endif
