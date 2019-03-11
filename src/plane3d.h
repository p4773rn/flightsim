#ifndef PLANE3D_H
#define PLANE3D_H

#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

/*
1.  Implement and test rotation integration

*/

struct Element {
    double mass;
    glm::dvec3 design_pos; // position in some weird coordinate system. Needed to obtain pos;
    glm::dvec3 inertia; // used to calculate intertia matrix of the plane
    double incidence; // pitch relative to plane
    double dihedral; // kinda like roll. Is 0 for all airfoild except rudder
    double area;
    double iflap;

    glm::dvec3 normal;
    glm::dvec3 pos; // relative to center of gravity
};


class Plane3D {
public:
    double mass;
    glm::dmat3 inertia;
    glm::dmat3 inertia_inv;

    glm::dvec3 pos;
    glm::dvec3 vel;
    glm::dvec3 vel_body;
    glm::dvec3 ang_vel;

    glm::dquat orientation;
    glm::dvec3 net_force;
    glm::dvec3 net_torque;

    Element airfoils[8]; 

    double thrust;
    bool flaps;
    double ipitch, iyaw, iroll;




    Plane3D();
    void calc_mass_properties();
    void update(double dt);
    glm::dvec3 get_angles() const;
    void calc_airplane_loads();



    std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> debug_arrows;
    std::vector<glm::dvec3> trail;
    double elapsed_time = 0;
    double last_trail_update = 0;
};



#endif
