#include "misc.h"
#include <algorithm>
#include <cmath>


double clamp(double val, double low, double high) {
    return std::max(low, std::min(high, val));
}

double rad2deg(double angle) {
    return angle / M_PI * 180;
}

double deg2rad(double angle) {
    return angle * M_PI / 180;
}

