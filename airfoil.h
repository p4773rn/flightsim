#ifndef AIRFOIL_H
#define AIRFOIL_H

#include "table.h"
#include "vec2.h"
#include <tuple>


class Airfoil {
public:
    Airfoil(const Table& tableDefaultFlaps, const Table& tableTakeOffFlaps, double area, double airfoilAngle, double chordLength) : 
        tableDefaultFlaps{tableDefaultFlaps},
        tableTakeOffFlaps{tableTakeOffFlaps},
        area{area},
        airfoilAngle{airfoilAngle},
        chordLength{chordLength},
        deflection{0},
        flaps{0}
    {}

    Airfoil(const Airfoil& other) = default;
    Airfoil(Airfoil&& other) = default;


    std::tuple<Vec2, double> getForceAndTorque(const Vec2& velocity, double angle, double airDensity) const;
    
    double getAngleOfAttack(const Vec2& velocity, double angle) const {
        return airfoilAngle 
               + deflection * (5.0 * M_PI / 180)
               + angle
               + atan2(-velocity.getY(), velocity.getX());
    }

    double getDeflection() const {return deflection;}
    void setDeflection(double deflection);
    double getFlaps() const {return flaps;}
    void setFlaps(double flapsPosition);

private:
    Table tableDefaultFlaps;
    Table tableTakeOffFlaps;
    double area;
    double airfoilAngle;
    double chordLength;

    // TODO: come up with better controls
    // TODO: make the deflections change continiously with time
    // TODO: come up with better names for deflection and flaps
    double deflection; // a number between -1 and 1
    double flaps; // [0, 0.25, 0.5, 0.75, 1] where 0 is minimal drag and 1 is take-off position
};

#endif
