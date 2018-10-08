#ifndef PLANE_H
#define PLANE_H

#include "airfoil.h"
#include "vec2.h"
#include "engine.h"

class Plane {
public:
    Plane() :
        engine(250000),
        wing(Table(
            {
                { -8.0 * M_PI / 180, -0.45, 0.022, 0.246, -0.097 },
                { -6.0 * M_PI / 180, -0.23, 0.014, 0.246, -0.092 },
                { -4.0 * M_PI / 180, -0.03, 0.012, 0.246, -0.092 },
                { -2.0 * M_PI / 180,  0.20, 0.010, 0.246, -0.092 },
                {  0.0 * M_PI / 180,  0.38, 0.010, 0.246, -0.093 },
                {  2.0 * M_PI / 180,  0.60, 0.010, 0.246, -0.095 },
                {  4.0 * M_PI / 180,  0.80, 0.012, 0.246, -0.098 },
                {  6.0 * M_PI / 180,  1.00, 0.014, 0.246, -0.100 },
                {  8.0 * M_PI / 180,  1.15, 0.017, 0.246, -0.100 },
                { 10.0 * M_PI / 180,  1.27, 0.022, 0.246, -0.095 },
                { 12.0 * M_PI / 180,  1.36, 0.030, 0.246, -0.092 },
                { 14.0 * M_PI / 180,  1.35, 0.042, 0.246, -0.092 },
                { 16.0 * M_PI / 180,  1.25, 0.059, 0.246, -0.095 }
            }),
            103, // Area
            0.0523598775, // Angle
            3.6), // Chord length
        elevators(Table(
            {
                { -14.0 * M_PI / 180, -0.87, 0.036, 0.25,  0.012 },
                { -12.0 * M_PI / 180, -0.89, 0.028, 0.25,  0.004 },
                { -10.0 * M_PI / 180, -0.90, 0.021, 0.25,  0.002 },
                {  -8.0 * M_PI / 180, -0.85, 0.018, 0.25,  0.0   },
                {  -6.0 * M_PI / 180, -0.64, 0.014, 0.25,  0.0   },
                {  -4.0 * M_PI / 180, -0.43, 0.011, 0.25,  0.0   },
                {  -2.0 * M_PI / 180, -0.21, 0.010, 0.25,  0.0   },
                {   0.0 * M_PI / 180,   0.0, 0.009, 0.25,  0.0   },
                {   2.0 * M_PI / 180,  0.21, 0.010, 0.25,  0.0   },
                {   4.0 * M_PI / 180,  0.43, 0.011, 0.25,  0.0   },
                {   6.0 * M_PI / 180,  0.64, 0.014, 0.25,  0.0   },
                {   8.0 * M_PI / 180,  0.85, 0.018, 0.25,  0.0   },
                {  10.0 * M_PI / 180,  0.90, 0.021, 0.25, -0.002 },
                {  12.0 * M_PI / 180,  0.89, 0.028, 0.25, -0.004 },
                {  14.0 * M_PI / 180,  0.87, 0.036, 0.25, -0.012 }
            }),
            33,
            -0.0104719755,
            2.6
            )
    {
        pos = Vec2(0, 100);
        velocity = Vec2(100, 0);
    }

    double getHeight() const {return pos.getY();}
    const Vec2& getPos() const {return pos;}
    void update(double delta);

private:
    Vec2 pos; // For now, xy coordinates of the plane
    Vec2 velocity;
    double angle = 0.0872665; // angle of plane relative to horizon
    double angularVelocity = 0;
    double mass = 51710;
    double inertia = 2027731;

    double wingsPoint = 2; // distance from wings' force to the mass center
    Airfoil wing;

    double elevatorsPoint = -15;
    Airfoil elevators;

    Engine engine;
};


#endif
