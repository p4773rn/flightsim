#ifndef MISC_H
#define MISC_H

#include <algorithm>

double rad2deg(double rad);
double deg2rad(double deg);

template<typename T>
const T& clamp(const T& v, const T& lo, const T& hi) {
    return std::min(hi, std::max(lo, v));
}

template<typename T>
T lerp(const T& left, const T& right, const T& weight) {
    return left * (1 - weight) + right * weight;
}


#endif
