#include "engine.h"
#include <algorithm>
#include <cmath>

void Engine::setThrottle(int throttle) {
    this->throttle = std::max(0, std::min(100, throttle));
}

Vec2 Engine::getThrust(double pitchAngle) const {
    double thrust = (maxThrust * throttle) / 100.0;
    return Vec2(thrust * cos(pitchAngle), thrust * sin(pitchAngle));
}
