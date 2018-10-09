#ifndef ENGINE_H
#define ENGINE_H

#include "vec2.h"

class Engine {
public:
    Engine(double maxThrust) : maxThrust(maxThrust)
        {}

    void setThrottle(int throttle);
    Vec2 getThrust(double angle) const;
private:
    int throttle = 0; // 0 5 10 15 ...
    double maxThrust;
};

#endif
