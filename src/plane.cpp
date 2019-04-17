#include "plane.h" 
#include "environment.h"
#include "misc.h"
#include <iostream>
#include <tuple>

using std::cout;
using std::endl;

void Plane::update(double delta,
        std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& debugArrows) {
    glm::dvec3 netForce(0), netTorque(0);
    
    std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> bodyArrows;
    for (auto& s : wings) {
        glm::dvec3 force, torque;
        std::tie(force, torque) = s.getForceAndTorque(vel, orientation, angVel, pos.y, bodyArrows);
        netForce += force;
        netTorque += torque;
    }
    for (auto& s : elevators) {
        glm::dvec3 force, torque;
        std::tie(force, torque) = s.getForceAndTorque(vel, orientation, angVel, pos.y, bodyArrows);
        netForce += force;
        netTorque += torque;
    }
    for (auto& s : rudder) {
        glm::dvec3 force, torque;
        std::tie(force, torque) = s.getForceAndTorque(vel, orientation, angVel, pos.y, bodyArrows);
        netForce += force;
        netTorque += torque;
    }
    for (auto& s : fuselage) {
        glm::dvec3 force, torque;
        std::tie(force, torque) = s.getForceAndTorque(vel, orientation, angVel, pos.y, bodyArrows);
        netForce += force;
        netTorque += torque;
    }

    bodyArrows.push_back(std::make_tuple(glm::dvec3(0,0,0), glm::dvec3(0,1,0), glm::dvec3(1,0,0)));

    debugArrows.push_back(std::make_tuple(pos, vel, glm::dvec3(1,0,0)));

    for (auto& arrow : bodyArrows) {
        glm::dvec3 arrowPos, arrowDir, arrowColor;
        std::tie(arrowPos, arrowDir, arrowColor) = arrow;
        arrowPos = pos + orientation * arrowPos;
        arrowDir = orientation * arrowDir;
        debugArrows.push_back(std::make_tuple(arrowPos, arrowDir, arrowColor));
    }

    cout << rudder.back().getDeflection().x << " ";
    cout << elevators.back().getDeflection().x << " " << getFlaps() << endl;
    // cout << orientation.w << ' ' << orientation.x << ' ' << orientation.y << ' ' << orientation.z << ' '  << endl;

    glm::vec3 euler = glm::eulerAngles(orientation);
    // cout << rad2deg(euler.y) << ' ' << rad2deg(euler.x) << ' ' << rad2deg(euler.z) << ' ' << endl;

    netForce += orientation * engine.getThrust();
    

    netForce.y += -9.8 * mass;

    
    glm::dvec3 frontWheelsForce, frontWheelsTorque;
    glm::dvec3 mainWheelsLForce, mainWheelsLTorque;
    glm::dvec3 mainWheelsRForce, mainWheelsRTorque;

    std::tie(frontWheelsForce, frontWheelsTorque)= frontWheels.getForceAndTorque(vel, pos, angVel, orientation, netForce, debugArrows);
    std::tie(mainWheelsLForce, mainWheelsLTorque)= mainWheelsL.getForceAndTorque(vel, pos, angVel, orientation, netForce, debugArrows);
    std::tie(mainWheelsRForce, mainWheelsRTorque)= mainWheelsR.getForceAndTorque(vel, pos, angVel, orientation, netForce, debugArrows);

    netForce += frontWheelsForce;
    netForce += mainWheelsLForce;
    netForce += mainWheelsRForce;
    netTorque += frontWheelsTorque;
    netTorque += mainWheelsLTorque;
    netTorque += mainWheelsRTorque;

    auto localVel = glm::conjugate(orientation) * vel; 
    // localVel.z = -forward speed
    // localVel.y = not sure what it means
    std::cout << pos << "\t " << localVel.z << "\t" << vel  << std::endl;

    glm::dvec3 acc = netForce / mass;
    vel += acc * delta;
    pos += vel * delta;

    glm::dvec3 angAcc = glm::inverse(inertia) * (netTorque - glm::cross(angVel, inertia * angVel));
    angVel += angAcc * delta;
    orientation += (orientation * glm::dquat(0, angVel)) * 0.5 * delta;
    orientation = glm::normalize(orientation);

}

Plane Plane::getDefaultPlane(std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& debugArrows, const Terrain& terrain) {
    // sources of specs
    
    // http://www.boeing.com/assets/pdf/commercial/airports/acaps/737.pdf
    // page 32

    // http://www.b737.org.uk/techspecsoriginals.htm

    // {w/e/r/f}Pos are design coordinates
    // I counted from the tip of the nose on x
    // and from wheels on y
    glm::dvec3 wPos(0.5*4, 0.5*4, 14);
    double wArea = 90;
    double wRoot = 7.32;
    double wTip = 1.60;
    double wIncidence = -glm::radians(1.5);
    double wDihedral = glm::radians(6.);
    double wSweep = glm::radians(25.);
    double wLength = 14.5;

    glm::dvec3 ePos(0, 3, 32);
    double eArea = 4*4;
    double eRoot = 4;
    double eTip = 2;
    double eIncidence = -glm::radians(0.);
    double eDihedral = glm::radians(7.);
    double eSweep = glm::radians(30.);
    double eLength = 2 * eArea/(eTip + eRoot);

    glm::dvec3 rPos(0, 6, 32);
    double rArea = 20.81;
    double rRoot = 1.2*4;
    double rTip = 0.5*4;
    double rIncidence = -glm::radians(0.);
    double rDihedral = glm::radians(90.);
    double rSweep = glm::radians(35.);
    double rLength = 2 * rArea/(rTip + rRoot);

    glm::dvec3 fPos(0, 4, 0);
    double fArea = 56;
    double fRoot = 26;
    double fTip = 30;
    double fIncidence = -glm::radians(0.);
    double fDihedral = glm::radians(0.);
    double fSweep = glm::radians(0.);
    // double fLength = 2 * fArea/(fTip + fRoot);
    double fLength = 1.88;

    double totalMass = 51000;

    std::vector<glm::dvec3> pointMasses;
    addPointMasses(wLength, wRoot, wTip, wSweep, wPos, wDihedral, false, pointMasses);
    addPointMasses(wLength, wRoot, wTip, wSweep, wPos, wDihedral, true, pointMasses);
    addPointMasses(eLength, eRoot, eTip, eSweep, ePos, eDihedral, false, pointMasses);
    addPointMasses(eLength, eRoot, eTip, eSweep, ePos, eDihedral, true, pointMasses);
    addPointMasses(rLength, rRoot, rTip, rSweep, rPos, rDihedral, false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, fDihedral, false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, fDihedral, true, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, glm::radians(90.), false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, glm::radians(-90.), false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, fDihedral, false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, fDihedral, true, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, glm::radians(90.), false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, glm::radians(-90.), false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, fDihedral, false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, fDihedral, true, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, glm::radians(90.), false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, glm::radians(-90.), false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, fDihedral, false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, fDihedral, true, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, glm::radians(90.), false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, glm::radians(-90.), false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, fDihedral, false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, fDihedral, true, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, glm::radians(90.), false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, glm::radians(-90.), false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, fDihedral, false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, fDihedral, true, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, glm::radians(90.), false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, glm::radians(-90.), false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, fDihedral, false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, fDihedral, true, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, glm::radians(90.), false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, glm::radians(-90.), false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, fDihedral, false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, fDihedral, true, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, glm::radians(90.), false, pointMasses);
    addPointMasses(fLength, fRoot, fTip, fSweep, fPos, glm::radians(-90.), false, pointMasses);
    double dm = totalMass / pointMasses.size();

    glm::dvec3 centerMass(0);
    for (auto& p : pointMasses)
        centerMass += p * dm;
    centerMass /= totalMass;

    for (auto& p : pointMasses)
        p -= centerMass;

    wPos -= centerMass;
    ePos -= centerMass;
    rPos -= centerMass;
    fPos -= centerMass;

    double Ixx=0, Iyy=0, Izz=0, Ixy=0, Ixz=0, Iyz=0;
    for (auto& p : pointMasses) {
        Ixx += (p.y*p.y + p.z*p.z) * dm;
        Iyy += (p.x*p.x + p.z*p.z) * dm;
        Izz += (p.x*p.x + p.y*p.y) * dm;
        Ixy += -p.x * p.y * dm;
        Ixz += -p.x * p.z * dm;
        Iyz += -p.y * p.z * dm;
        debugArrows.push_back(std::make_tuple(p, glm::dvec3(0,1,0), glm::dvec3(0,0,1)));
    }
    glm::dmat3 inertia;
    inertia[0][0] = Ixx;
    inertia[1][1] = Iyy;
    inertia[2][2] = Izz;
    inertia[0][1] = inertia[1][0] = Ixy;
    inertia[0][2] = inertia[2][0] = Ixz;
    inertia[1][2] = inertia[2][1] = Iyz;


    Table wingsTable({
        { glm::radians(-8.0),  -0.8,  0.027, 0.25, -0.018},
        { glm::radians(-6.0), -0.56,  0.015, 0.25, -0.005},
        { glm::radians(-4.0),  -0.4,  0.012, 0.25, -0.005},
        { glm::radians(-2.0),     0,  0.011, 0.25,  -0.01},
        { glm::radians(0.0),  0.15,   0.01, 0.25, -0.003},
        { glm::radians(2.0),   0.3,  0.012, 0.25, -0.005},
        { glm::radians(4.0),  0.61,  0.014, 0.25, -0.008},
        { glm::radians(6.0),   0.8,  0.017, 0.25, -0.003},
        { glm::radians(8.0),  1.05,  0.022, 0.25, 0.0025},
        { glm::radians(10.0),  1.15,  0.027, 0.25,  0.007},
        { glm::radians(12.0),  1.25,  0.032, 0.25,  0.013},
        { glm::radians(14.0),   1.3,   0.05, 0.25,  0.024},
        { glm::radians(16.0),   0.8,   0.12, 0.25,      0},
    });

    Table elevatorsTable({
        { glm::radians(-14.0), -0.87, 0.036, 0.25,  0.012 },
        { glm::radians(-12.0), -0.89, 0.028, 0.25,  0.004 },
        { glm::radians(-10.0), -0.90, 0.021, 0.25,  0.002 },
        { glm::radians( -8.0), -0.85, 0.018, 0.25,  0.0   },
        { glm::radians( -6.0), -0.64, 0.014, 0.25,  0.0   },
        { glm::radians( -4.0), -0.43, 0.011, 0.25,  0.0   },
        { glm::radians( -2.0), -0.21, 0.010, 0.25,  0.0   },
        { glm::radians(  0.0),   0.0, 0.009, 0.25,  0.0   },
        { glm::radians(  2.0),  0.21, 0.010, 0.25,  0.0   },
        { glm::radians(  4.0),  0.43, 0.011, 0.25,  0.0   },
        { glm::radians(  6.0),  0.64, 0.014, 0.25,  0.0   },
        { glm::radians(  8.0),  0.85, 0.018, 0.25,  0.0   },
        { glm::radians( 10.0),  0.90, 0.021, 0.25, -0.002 },
        { glm::radians( 12.0),  0.89, 0.028, 0.25, -0.004 },
        { glm::radians( 14.0),  0.87, 0.036, 0.25, -0.012 },
    });

    Table rudderTable({
        { glm::radians(-14.0), -0.87 * 5.2, 0.036, 0.25,  0.012 },
        { glm::radians(-12.0), -0.89 * 5.2, 0.028, 0.25,  0.004 },
        { glm::radians(-10.0), -0.90 * 5.2, 0.021, 0.25,  0.002 },
        { glm::radians( -8.0), -0.85 * 5.2, 0.018, 0.25,  0.0   },
        { glm::radians( -6.0), -0.64 * 5.2, 0.014, 0.25,  0.0   },
        { glm::radians( -4.0), -0.43 * 5.2, 0.011, 0.25,  0.0   },
        { glm::radians( -2.0), -0.21 * 5.2, 0.010, 0.25,  0.0   },
        { glm::radians(  0.0),   0.0 * 5.2, 0.009, 0.25,  0.0   },
        { glm::radians(  2.0),  0.21 * 5.2, 0.010, 0.25,  0.0   },
        { glm::radians(  4.0),  0.43 * 5.2, 0.011, 0.25,  0.0   },
        { glm::radians(  6.0),  0.64 * 5.2, 0.014, 0.25,  0.0   },
        { glm::radians(  8.0),  0.85 * 5.2, 0.018, 0.25,  0.0   },
        { glm::radians( 10.0),  0.90 * 5.2, 0.021, 0.25,  0.002 },
        { glm::radians( 12.0),  0.89 * 5.2, 0.028, 0.25,  0.004 },
        { glm::radians( 14.0),  0.87 * 5.2, 0.036, 0.25,  0.012 },
    });

    Table fuselageTable({
        { glm::radians(  0.0),   0.0,   1.0, 0.25,  0.0 },
    });

    std::vector<AirfoilSegment> wings = createAirfoilSegments(10, wLength, wRoot, wTip, wSweep, wPos, wIncidence, wDihedral, false, wingsTable);
    std::vector<AirfoilSegment> leftWings = createAirfoilSegments(10, wLength, wRoot, wTip, wSweep, wPos, wIncidence, wDihedral, true, wingsTable);
    wings.insert(wings.end(), leftWings.begin(), leftWings.end());

    std::vector<AirfoilSegment> elevators = createAirfoilSegments(5, eLength, eRoot, eTip, eSweep, ePos, eIncidence, eDihedral, false, elevatorsTable);
    std::vector<AirfoilSegment> leftElevators = createAirfoilSegments(5, eLength, eRoot, eTip, eSweep, ePos, eIncidence, eDihedral, true, elevatorsTable);
    elevators.insert(elevators.end(), leftElevators.begin(), leftElevators.end());

    std::vector<AirfoilSegment> rudder = createAirfoilSegments(1, rLength, rRoot, rTip, rSweep, rPos, rIncidence, rDihedral, false, rudderTable);

    std::vector<AirfoilSegment> fuselage;// = createAirfoilSegments(1, fLength, fRoot, fTip, fSweep, glm::dvec3, fIncidence, fDihedral, false, fuselageTable);


    // let's ignore rotation moment for now


    Plane plane = Plane(
        glm::dvec3(0, 4, 0), // pos
        glm::dvec3(0, 0, 0),// velocity
        totalMass,
        inertia,
        
        std::move(wings),
        std::move(elevators),
        std::move(rudder),
        std::move(fuselage),
        Engine(105000), // engine
        Wheels(72467, 10, 10000, glm::dvec3(0, -4, -10), terrain), // Front wheels
        Wheels(434802, 400, 40000, glm::dvec3(-3, -4, 5), terrain), // Main wheels left
        Wheels(434802, 400, 40000, glm::dvec3(3, -4, 5), terrain) // Main wheels right
    );
    plane.orientation = glm::dquat(glm::dvec3(0,1,0));
    plane.vel = plane.orientation * glm::dvec3(0,0,-100);

    return plane;
}


void Plane::addPointMasses(double length, double root, double tip, double sweep, 
                           const glm::dvec3& pos, double dihedral, bool invertX,
                           std::vector<glm::dvec3>& pointMasses) {
    double d = 0.5; // step size in meters 
    double qlength = length / glm::cos(sweep); // length of quarter chord line
    double ledge0 = 0;
    double ledge1 = 0.25 * root + glm::sin(sweep) * qlength;

    double scaleX = invertX ? -1. : 1.;

    for (double l = 0; l < length; l += d) {
        double clength = root + (tip - root) * (l/length); // chord length
        for (double c = 0; c < clength; c += d) {
            glm::dvec3 point;
            point.x = glm::cos(dihedral) * l;
            point.y = glm::sin(dihedral) * l;
            point.z = c + (ledge0 + (ledge1 - ledge0) * (l/length));
            point += pos;
            point.x *= scaleX;

            pointMasses.push_back(point);
        }
    }
}

double Plane::getPitchAngle() const {
	double angle = glm::eulerAngles(orientation).x * 180.0 / M_PI;
	if (angle > 90) angle = angle - 180;
	if (angle < -90) angle = (180 + angle);
	return angle;
}

double Plane::getYawAngle() const {
	double angle = glm::eulerAngles(orientation).y * 180.0 / M_PI;
	return angle;
}

std::vector<AirfoilSegment> Plane::createAirfoilSegments(
                               int num_segments,
                               double length, double root, double tip, double sweep, 
                               const glm::dvec3& pos, double incidence, double dihedral, bool invertX,
                               const Table& tableFlaps0) {
    std::vector<AirfoilSegment> segments; 
    
    double qlength = length / glm::cos(sweep); // length of quarter chord line
    double ledge0 = 0;
    double ledge1 = 0.25 * root + glm::sin(sweep) * qlength;

    double scaleX = invertX ? -1. : 1.;

    for (int li = 0; li < num_segments; ++li) {
        double length_ratio = ((double)li+0.5)/num_segments;
        double dl = length/num_segments;
        double clength = root + (tip - root) * length_ratio; // chord length
        double area = clength * dl;

        glm::dvec3 offset;
        offset.x = glm::cos(dihedral) * dl * (li + 0.5);
        offset.y = glm::sin(dihedral) * dl * (li + 0.5);
        offset.z = clength/2 + (ledge0 + (ledge1 - ledge0) * length_ratio);
        glm::dvec2 angle(incidence, dihedral * scaleX);

        glm::dvec3 segmentPos = pos + offset;
        segmentPos.x *= scaleX;

        AirfoilSegment segment(tableFlaps0, segmentPos, angle, area);
        segments.push_back(segment);
    }

    return segments;
}

