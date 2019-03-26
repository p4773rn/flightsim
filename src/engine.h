#ifndef ENGINE_H
#define ENGINE_H

#include <glm/glm.hpp>

class Engine {
public:
    Engine(double maxThrust) : maxThrust{maxThrust}
        {}

    int getThrottle() const {return throttle;}
    void setThrottle(int throttle);
    glm::dvec3 getThrust() const;
private:
    int throttle = 0; // 0 5 10 15 ...
    double maxThrust;
};

#endif
