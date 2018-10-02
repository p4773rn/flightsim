#ifndef AIRFOIL_H
#define AIRFOIL_H

#include "table.h"
#include "vec2.h"

class Airfoil {
public:
    Airfoil(const Table& table, double area, double airfoilAngle) : 
        table(table),
        area(area),
        airfoilAngle(airfoilAngle)
    {

    }
    Vec2 getLift(const Vec2& velocity, double angle, double airPressure);
private:
    // Warning: unlike Hans' code this doesn't convert degrees to radians
    //Table table = {
    //    { -4.0,  -0.14,   0.014,  0,   -0.036 },
    //    { -2.0,   0.02,   0.008,  0,   -0.052 },
    //    {  0.0,   0.14,   0.008,  0,   -0.090 },
    //    {  2.0,   0.32,   0.012,  0,   -0.130 },
    //    {  4.0,   0.46,   0.020,  0,   -0.160 },
    //    {  6.0,   0.60,   0.030,  0,   -0.200 },
    //    {  8.0,   0.76,   0.044,  0,   -0.240 },
    //    { 10.0,   0.90,   0.060,  0,   -0.280 },
    //    { 12.0,   1.04,   0.070,  0,   -0.310 },
    //    { 14.0,   1.16,   0.096,  0,   -0.330 },
    //    { 15.0,   1.22,   0.110,  0,   -0.340 },
    //    { 16.0,   1.16,   0.140,  0,   -0.350 },
    //    { 18.0,   1.02,   0.210,  0,   -0.384 },
    //    { 20.0,   0.94,   0.260,  0,   -0.390 }
    //};
    Table table;
    double area;
    double airfoilAngle;
};

#endif
