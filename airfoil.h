#ifndef AIRFOIL_H
#define AIRFOIL_H

#include "table.h"
#include "vec2.h"
#include <tuple>


class Airfoil {
public:
    Airfoil(const Table& table, double area, double airfoilAngle, double chordLength) : 
        table{table},
        area{area},
        airfoilAngle{airfoilAngle},
        chordLength{chordLength}
    {}

    Airfoil(const Airfoil& other) = default;
    Airfoil(Airfoil&& other) = default;


    std::tuple<Vec2, double> getForceAndTorque(const Vec2& velocity, double angle, double airDensity) const;
    
    double getAngleOfAttack(const Vec2& velocity, double angle) const {
        return airfoilAngle
               + angle
               + atan2(-velocity.getY(), velocity.getX());
    }

    // double getWingsArea() {return this->area;}
    // double getWingsChordLength() {return this->chordLength;}
    // Table getTable() {return this->table;}
private:
    Table table;
    double area;
    double airfoilAngle;
    double chordLength;
};

#endif
