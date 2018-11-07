#ifndef FLIGHTSIM_MISC_H
#define FLIGHTSIM_MISC_H

// miscellaneous functions


double clamp(double val, double low, double high);

double rad2deg(double angle);
double deg2rad(double angle);

// linear interpolation
template<typename T>
T lerp(T from, T to, double weight) {
    return from * (1 - weight) + to * weight;
}


#endif
