#ifndef VEC2_H
#define VEC2_H

class Vec2 {
public:
    Vec2(float x, float y) : x(x), y(y)
        {}

    Vec2() : x(0), y(0)
        {}

    float getX() const {return x;}
    float getY() const {return y;}

    void setX(float x) {this->x = x;}
    void setY(float y) {this->y = y;}

    void operator+=(const Vec2& v) {
        x += v.x;
        y += v.y;
    }
    Vec2 operator * ( float d ) {
        return {x * d, y * d};
    }
    Vec2 operator / ( float d ) {
        return {x / d, y / d};
    }

private:
    float x;
    float y;
};

#endif
