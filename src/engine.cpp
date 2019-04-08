#include "engine.h"
#include <algorithm>
#include <cmath>

void Engine::setThrottle(int throttle) {
    this->throttle = std::max(0, std::min(100, throttle));
}

glm::dvec3 Engine::getThrust() const {
    double thrust = (maxThrust * throttle) / 100.0;
    return glm::dvec3(0, 0, -thrust);
}
