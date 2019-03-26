#include "plane.h" 
#include "environment.h"
#include "misc.h"
#include <iostream>
#include <tuple>

using std::cout;
using std::endl;

void Plane::update(double delta,
        std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& debugArrows) {
    std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> bodyArrows;
    for (auto& s : wings)
        s.getForceAndTorque(vel, angVel, pos.y, bodyArrows);
    for (auto& s : elevators)
        s.getForceAndTorque(vel, angVel, pos.y, bodyArrows);
    for (auto& s : rudder)
        s.getForceAndTorque(vel, angVel, pos.y, bodyArrows);
    for (auto& s : fuselage)
        s.getForceAndTorque(vel, angVel, pos.y, bodyArrows);



    debugArrows.insert(debugArrows.end(), bodyArrows.begin(), bodyArrows.end());

    //double airDensity = getAirDensity(pos.getY());
    //
    //Vec2 wingsForce, elevatorsForce;
    //double wingsTorque, elevatorsTorque;
    //
    //double height = getHeight();
    //std::tie(wingsForce, wingsTorque) = wings.getForceAndTorque(velocity, pitchAngle, airDensity, height);

    //Vec2 elevatorsVelocity = {
    //    velocity.getX() - angularVelocity * (elevatorsPoint.getX() * sin(pitchAngle) + 
    //                                         elevatorsPoint.getY() * cos(pitchAngle)),
    //    velocity.getY() + angularVelocity * (elevatorsPoint.getX() * cos(pitchAngle) - 
    //                                         elevatorsPoint.getY() * sin(pitchAngle))};
    //
    //std::tie(elevatorsForce, elevatorsTorque) = elevators.getForceAndTorque(elevatorsVelocity, pitchAngle, airDensity, height);
    //
    //// Forces update
    //Vec2 netForce;
    //
    //// Weight
    //Vec2 weight = Vec2(0, -GRAVITATIONAL_ACCELERATION * mass);
    //netForce += weight;
    //
    //// Lift + Drag
    //netForce += wingsForce;
    //netForce += elevatorsForce;

    //// Thrust
    //Vec2 thrust = engine.getThrust(pitchAngle);
    //netForce += thrust;
   
    //// Wheels force
    //Vec2 frontWheelsForce, mainWheelsForce;
    //double frontWheelsTorque, mainWheelsTorque;

    //std::tie(frontWheelsForce, frontWheelsTorque) = frontWheels.getForceAndTorque(velocity, pos, angularVelocity, pitchAngle, mass);
    //std::tie(mainWheelsForce, mainWheelsTorque) = mainWheels.getForceAndTorque(velocity, pos, angularVelocity, pitchAngle, mass);

    //cout << endl << "FRONT WHEELS FORCE: " << frontWheelsForce << endl << endl;
    //cout << endl << "MAIN WHEELS FORCE: " << mainWheelsForce << endl << endl;
    //netForce += frontWheelsForce;
    //netForce += mainWheelsForce;

    //// Straightforward implementation of drag from fuselage
    //// TODO, implement fuselage as airfoil (to produce more realistic drag)
    //// dynamicPressure * area * coefficients.drag;
    //double dynamicPressure = 0.5 * airDensity * velocity.lengthSquared();
    //double fuselageArea = 4 * 4; // Simple square for now
    //double fuselageDragCoef = 0.11; // Experimentally obtained
    //double fuselageDragMagnitude = dynamicPressure * fuselageArea * fuselageDragCoef;
    //Vec2 fuselageDrag = - velocity.normalized() * fuselageDragMagnitude;
    //netForce += fuselageDrag;

    //// Torque update
    //double torque = 0;
    //
    //torque += wingsTorque + wingsPoint.cross(wingsForce);
    //torque += elevatorsTorque + elevatorsPoint.cross(elevatorsForce);

    //// Wheels torque
    //torque += frontWheelsTorque + mainWheelsTorque;

    //double angularAcceleration = torque / inertia;
    //angularVelocity += angularAcceleration * delta;   
    //pitchAngle += angularVelocity * delta;

    //// Making sure pitch angle stays within [-pi, pi] rads
    //if (pitchAngle > M_PI)
    //    pitchAngle -= 2 * M_PI;
    //if (pitchAngle < -M_PI)
    //    pitchAngle += 2 * M_PI;

    //// Final calculations
    //Vec2 acceleration = netForce / mass;
    //velocity += acceleration * delta;
    //pos += velocity * delta;

    ////cout << pos.getX() << " " << pos.getY() << endl;
    //cout << "Pos: " << pos << endl;
    //cout << "Vel: " << velocity << endl;
    //cout << "Pitch Angle: " << pitchAngle / M_PI * 180 << " deg" << endl;
    //cout << "AngVel: " << angularVelocity / M_PI * 180 << " deg/sec" << endl;
    //cout << "Wings AoA: " << getWingsAoA() << " deg" << endl;
    //cout << "Elevators AoA: " << getElevatorsAoA() << " deg"  << endl;
    //cout << "--------------------------------------" << endl;
    //cout << "Gravity: " << weight << endl;
    //cout << "Wings: " << wingsForce << " " << wingsTorque + wingsPoint.cross(wingsForce) << endl;
    //cout << "Engines: " << thrust << endl;
    //cout << "Elevators: " << elevatorsForce << " " << elevatorsTorque + elevatorsPoint.cross(elevatorsForce) << endl;
    //cout << "Fuselage: " << fuselageDrag << endl;
    //cout << "Total: " << netForce << " " << torque << endl;
    //cout << "--------------------------------------" << endl;
    //cout << "Air density: " << airDensity << endl;
    //cout << "Ground effect multiplier: " << getGroundEffect(height) << endl;
    //cout << "--------------------------------------" << endl;
    //cout << "Throttle: " << engine.getThrottle() << endl;
    //cout << "Elevator Deflection: " << elevators.getDeflection() << endl;
    //cout << "Flaps position: " << wings.getFlaps() << endl;
    //cout << "Brakes on/off: " << mainWheels.getBrakesStatus() << endl;
    //cout << endl;
}

Plane Plane::getDefaultPlane(std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& debugArrows) {
    // sources of specs
    
    // http://www.boeing.com/assets/pdf/commercial/airports/acaps/737.pdf
    // page 32

    // http://www.b737.org.uk/techspecsoriginals.htm

    // {w/e/r/f}Pos are design coordinates
    // I counted from the tip of the nose on x
    // and from wheels on y
    glm::dvec3 wPos(0.5*4,0.5*4,2.5*4);
    double wArea = 90;
    double wRoot = 7.32;
    double wTip = 1.60;
    double wIncidence = glm::radians(10.);
    double wDihedral = glm::radians(6.);
    double wSweep = glm::radians(25.);
    double wLength = 2 * wArea/(wTip + wRoot);

    glm::dvec3 ePos(0,1.2*4,6.25*4);
    double eArea = 4*4;
    double eRoot = 4;
    double eTip = 2;
    double eIncidence = glm::radians(0.);
    double eDihedral = glm::radians(7.);
    double eSweep = glm::radians(30.);
    double eLength = 2 * eArea/(eTip + eRoot);

    glm::dvec3 rPos(0,1.2*4,6.0*4);
    double rArea = 20.81;
    double rRoot = 1.2*4;
    double rTip = 0.5*4;
    double rIncidence = glm::radians(0.);
    double rDihedral = glm::radians(90.);
    double rSweep = glm::radians(35.);
    double rLength = 2 * rArea/(rTip + rRoot);

    glm::dvec3 fPos(0,0.75*4,0);
    double fArea = 50;
    double fRoot = 7*4;
    double fTip = 5*4;
    double fIncidence = glm::radians(0.);
    double fDihedral = glm::radians(0.);
    double fSweep = glm::radians(0.);
    double fLength = 2 * fArea/(fTip + fRoot);

    double totalMass = 40000;

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

    double dm = totalMass / pointMasses.size();

    double Ixx=0, Iyy=0, Izz=0, Ixy=0, Ixz=0, Iyz=0;
    for (auto& p : pointMasses) {
        Ixx += (p.y*p.y + p.z*p.z) * dm;
        debugArrows.push_back(std::make_tuple(p, glm::dvec3(0,1,0), glm::dvec3(0,0,1)));
    }


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
        { glm::radians(-14.0), -0.87, 0.036, 0.25,  0.012 },
        { glm::radians(-12.0), -0.89, 0.028, 0.25,  0.004 },
        { glm::radians(-10.0), -0.90, 0.021, 0.25,  0.002 },
        { glm::radians( -8.0), -0.85, 0.018, 0.25,  0.0   },
        { glm::radians( -6.0), -0.64, 0.014, 0.25,  0.0   },
        { glm::radians( -4.0), -0.43, 0.011, 0.25,  0.0   },
        { glm::radians( -2.0), -0.21, 0.010, 0.25,  0.0   },
        { glm::radians(  0.0),   0.0, 0.009, 0.25,  0.0   },
        { glm::radians(  2.0), -0.21, 0.010, 0.25,  0.0   },
        { glm::radians(  4.0), -0.43, 0.011, 0.25,  0.0   },
        { glm::radians(  6.0), -0.64, 0.014, 0.25,  0.0   },
        { glm::radians(  8.0), -0.85, 0.018, 0.25,  0.0   },
        { glm::radians( 10.0), -0.90, 0.021, 0.25,  0.002 },
        { glm::radians( 12.0), -0.89, 0.028, 0.25,  0.004 },
        { glm::radians( 14.0), -0.87, 0.036, 0.25,  0.012 },
    });

    Table fuselageTable({
        { glm::radians(  0.0),   0.0,   1.0, 0.25,  0.0 },
    });

    std::vector<AirfoilSegment> wings = createAirfoilSegments(10, wLength, wRoot, wTip, wSweep, wPos, wIncidence, wDihedral, false, wingsTable);
    std::vector<AirfoilSegment> elevators;
    std::vector<AirfoilSegment> rudder;
    std::vector<AirfoilSegment> fuselage;


    // let's ignore rotation moment for now

    cout<<wings.size()<<endl;

    float mass = 51000;
    glm::dmat3 inertia(1);

    Plane plane = Plane(
        glm::dvec3(0, 5, 0), // pos
        glm::dvec3(0),// velocity
        glm::dvec3(0), // angles
        mass,
        inertia,
        
        std::move(wings),
        std::move(elevators),
        std::move(rudder),
        std::move(fuselage),
        Engine(105000), // engine
        Wheels(72467, 10, 10000, Vec2(15, -2)), // Front wheels
        Wheels(434802, 400, 40000, Vec2(-2, -2)) // Main wheels
    );

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
        double length_ratio = ((double)li)/num_segments;
        double dl = length/num_segments;
        double clength = root + (tip - root) * length_ratio; // chord length
        double area = clength * dl;

        glm::dvec3 offset(dl * li, 0, clength/2 + (ledge0 + (ledge1 - ledge0) * length_ratio));
        glm::dvec2 angle(incidence, dihedral);

        AirfoilSegment segment(tableFlaps0, pos+offset, angle, area);
        segments.push_back(segment);
    }

    return segments;
}
