#include "plane3d.h"
#include <iostream>
#include "misc.h"

using std::cout;
using std::endl;

Plane3D::Plane3D() {
    pos = glm::dvec3(0, 300, 0);
    vel = glm::dvec3(0, 0, 0);
    ang_vel = glm::dvec3(0, 0, 0);

    net_force = glm::dvec3(0, 0, 0);
    thrust = 0;

    net_torque = glm::dvec3(0, 0, 0);
    
    vel_body = glm::dvec3(0, 0, 0);

    flaps = false;
    
    ipitch = 0;
    iyaw = 1;
    iroll = 0.0;
    orientation = glm::dquat(glm::dvec3(ipitch, iyaw, iroll));
    vel = orientation * glm::dvec3(0, 0, -60);

    calc_mass_properties();
}

void Plane3D::calc_mass_properties() {
    double total_mass;
    glm::dvec3 moment;
    glm::dvec3 center_gravity;
    double ixx, iyy, izz, ixy, ixz, iyz;

    // the book didn't specify units
    // these number might be in slugs, feet, etc.
    // incidence and dihedral are in degrees
    // design_pos are in forward,right,up system
    // our coordinate system is right,up,backward


    // left wing section with ailerons
    airfoils[0].mass = 6.56;
    airfoils[0].design_pos = glm::dvec3(14.5,12.0,2.5);
    airfoils[0].inertia = glm::dvec3(13.92,10.50,24.00);
    airfoils[0].incidence = -3.5;
    airfoils[0].dihedral = 0.0;
    airfoils[0].area = 31.2;
    airfoils[0].iflap = 0;

    // left wing section with flaps
    airfoils[1].mass = 7.31;
    airfoils[1].design_pos = glm::dvec3(14.5,5.5,2.5);
    airfoils[1].inertia = glm::dvec3(21.95,12.22,33.67);
    airfoils[1].incidence = -3.5;
    airfoils[1].dihedral = 0.0;
    airfoils[1].area = 36.4;
    airfoils[1].iflap = 0;
   
    // right wing section with flaps
    airfoils[2].mass = 7.31;
    airfoils[2].design_pos = glm::dvec3(14.5,-5.5,2.5);
    airfoils[2].inertia = glm::dvec3(21.95,12.22,33.67);
    airfoils[2].incidence = -3.5;
    airfoils[2].dihedral = 0.0;
    airfoils[2].area = 36.4;
    airfoils[2].iflap = 0;
   
    // left wing section with ailerons
    airfoils[3].mass = 6.56;
    airfoils[3].design_pos = glm::dvec3(14.5,-12.0,2.5);
    airfoils[3].inertia = glm::dvec3(13.92,10.50,24.00);
    airfoils[3].incidence = -3.5;
    airfoils[3].dihedral = 0.0;
    airfoils[3].area = 31.2;
    airfoils[3].iflap = 0;
   
    // left elevator
    airfoils[4].mass = 2.62;
    airfoils[4].design_pos = glm::dvec3(3.03,2.5,3.0);
    airfoils[4].inertia = glm::dvec3(0.837,0.385,1.206);
    airfoils[4].incidence = 0.0;
    airfoils[4].dihedral = 0.0;
    airfoils[4].area = 10.8;
    airfoils[4].iflap = 0;
        
    // right elevator
    airfoils[5].mass = 2.62;
    airfoils[5].design_pos = glm::dvec3(3.03,-2.5,3.0);
    airfoils[5].inertia = glm::dvec3(0.837,0.385,1.206);
    airfoils[5].incidence = 0.0;
    airfoils[5].dihedral = 0.0;
    airfoils[5].area = 10.8;
    airfoils[5].iflap = 0;
   
    // rudder
    airfoils[6].mass = 2.93;
    airfoils[6].design_pos = glm::dvec3(2.25,0.0,5.0);
    airfoils[6].inertia = glm::dvec3(1.262,1.942,0.718);
    airfoils[6].incidence = 0.0;
    airfoils[6].dihedral = 90.0;
    airfoils[6].area = 12.0;
    airfoils[6].iflap = 0;
   
    // fuselage
    airfoils[7].mass = 31.8;
    airfoils[7].design_pos = glm::dvec3(15.25,0.0,1.5);
    airfoils[7].inertia = glm::dvec3(66.30,861.9,861.9);
    airfoils[7].incidence = 0.0;
    airfoils[7].dihedral = 0.0;
    airfoils[7].area = 84.0;
    airfoils[7].iflap = 0;


    // convert to our coordinate system
    for (int i = 0; i < 8; ++i) {
        auto& a = airfoils[i];
        a.design_pos = glm::dvec3(a.design_pos.y, a.design_pos.z, -a.design_pos.x);
    }


    for (int i = 0; i < 8; ++i) {
        double in = glm::radians(airfoils[i].incidence);
        double di = glm::radians(airfoils[i].dihedral);
        airfoils[i].normal = glm::normalize(glm::dvec3(
            cos(in)*sin(di),
            cos(in)*cos(di),
            -sin(in)
        ));
    }

    total_mass = 0;
    for (int i = 0; i < 8; ++i)
        total_mass += airfoils[i].mass;

    moment = glm::dvec3(0, 0, 0);
    for (int i = 0; i < 8; ++i)
        moment += airfoils[i].mass * airfoils[i].design_pos;

    center_gravity = moment / total_mass;

    for (int i = 0; i < 8; ++i)
        airfoils[i].pos = airfoils[i].design_pos - center_gravity;


    ixx = 0; iyy = 0; izz = 0;
    ixy = 0; ixz = 0; iyz = 0;

    for (int i = 0; i < 8; ++i) {
        const Element& a = airfoils[i];
        ixx += a.inertia.x + a.mass *
                (a.pos.y * a.pos.y + a.pos.z * a.pos.z);
        iyy += a.inertia.y + a.mass *
                (a.pos.z * a.pos.z + a.pos.x * a.pos.x);
        izz += a.inertia.z + a.mass *
                (a.pos.x * a.pos.x + a.pos.y * a.pos.y);
        ixy += a.mass * (a.pos.x * a.pos.y);
        ixz += a.mass * (a.pos.x * a.pos.z);
        iyz += a.mass * (a.pos.y * a.pos.z);
    }

    mass = total_mass;
    inertia[0][0] = ixx;
    inertia[0][1] = -ixy;
    inertia[0][2] = -ixz;
    inertia[1][0] = -ixy;
    inertia[1][1] = iyy;
    inertia[1][2] = -iyz;
    inertia[2][0] = -ixz;
    inertia[2][1] = -iyz;
    inertia[2][2] = izz;

    inertia_inv = glm::inverse(inertia);
}

glm::dvec3 rotate(const glm::dquat& q, const glm::dvec3& v) {
    return q * v;
}



void Plane3D::update(double dt) {
    debug_arrows.clear();


    cout << endl << endl << endl << endl << endl;
    cout << "pos: " << pos << endl;
    cout << "vel: " << vel << endl;
    cout << "--------------------" << endl;
    cout << "ipitch: " << ipitch << endl;
    cout << "iyaw: " << iyaw << endl;
    cout << "iroll: " << iroll << endl;
    cout << "flaps: " << flaps << endl;
    cout << "thrust: " << thrust << endl;

    elapsed_time += dt;
    if (elapsed_time - last_trail_update > .5) {
        trail.push_back(pos);
        last_trail_update = elapsed_time;
    }
    for (size_t i = 0; trail.size() > 1 && i < trail.size() - 1; ++i) {
        debug_arrows.push_back({
            glm::vec3(trail[i]),
            glm::vec3(trail[i+1] - trail[i]),
            glm::vec3(0,1,1)
        });
    }

    
    // body axes
    debug_arrows.push_back({
        glm::vec3(pos), 
        glm::vec3(glm::normalize(rotate(orientation, glm::vec3(1,0,0)))*20.0),
        glm::vec3(1,0,0)
    });
    debug_arrows.push_back({
        glm::vec3(pos), 
        glm::vec3(glm::normalize(rotate(orientation, glm::vec3(0,1,0)))*20.0),
        glm::vec3(0,1,0)
    });
    debug_arrows.push_back({
        glm::vec3(pos), 
        glm::vec3(glm::normalize(rotate(orientation, glm::vec3(0,0,1)))*20.0),
        glm::vec3(0,0,1)
    });
    

    // airfoil normals
    for (int i = 0; i < 8; ++i) {
        auto& a = airfoils[i];
        debug_arrows.push_back({
            glm::vec3(pos + rotate(orientation, a.pos)), 
            glm::vec3(glm::normalize(rotate(orientation, a.normal))*5.0),
            glm::vec3(1,1,0)
        });
    }



    calc_airplane_loads();

    glm::dvec3 acc = net_force / mass;


    vel += acc * dt;
    pos += vel * dt;


    glm::dvec3 ang_acc = inertia_inv * (net_torque - glm::cross(ang_vel, inertia * ang_vel));
    ang_vel += ang_acc * dt;
    orientation += (orientation * glm::dquat(0, ang_vel)) * 0.5 * dt;
    orientation = glm::normalize(orientation);



}



// TODO: fix quaternion to euler angle conversion
// Most code from googling and glm::eulerAngles
// are either broken or I am doing smth wrong (which is impossible lol)
// (plane reverses direction of rotation after 180deg)
// I tried code from wiki, physics for gamedevs, Martin Baker webpage
glm::dvec3 Plane3D::get_angles() const {
    // convert quaternion to euler angles    
    glm::dvec3 angles = glm::eulerAngles(orientation);
    auto& q = orientation; 

    float h, a, b;
    h = atan2(2*q.y*q.w-2*q.x*q.z , 1 - 2*q.y*q.y - 2*q.z*q.z);
    a = asin(2*q.x*q.y + 2*q.z*q.w);
    b = atan2(2*q.x*q.w-2*q.y*q.z , 1 - 2*q.x*q.x - 2*q.z*q.z);
    
    if (abs(q.x*q.y + q.z*q.w - 0.5) < 0.0001) {
        h = 2 * atan2(q.x,q.w);
        b = 0;
    }
    
    if (abs(q.x*q.y + q.z*q.w - (-0.5)) < 0.0001) {
        h = -2 * atan2(q.x,q.w);
        b = 0; 
    }


    return glm::dvec3(h,a,b);
}







float LiftCoefficient(float angle, int flaps)
{
    float clf0[9] = {-0.54f, -0.2f, 0.2f, 0.57f, 0.92f, 1.21f, 1.43f, 1.4f, 1.0f};
    float clfd[9] = {0.0f, 0.45f, 0.85f, 1.02f, 1.39f, 1.65f, 1.75f, 1.38f, 1.17f};
    float clfu[9] = {-0.74f, -0.4f, 0.0f, 0.27f, 0.63f, 0.92f, 1.03f, 1.1f, 0.78f};
    float a[9] = {-8.0f, -4.0f, 0.0f, 4.0f, 8.0f, 12.0f, 16.0f, 20.0f, 24.0f};
    float cl;
    int i;
    cl = 0;
    for (i=0; i<8; i++)
    {
        if( (a[i] <= angle) && (a[i+1] > angle) )
        {
            switch(flaps)
            {
                case 0:// flaps not deflected
                    cl = clf0[i] - (a[i] - angle) * (clf0[i] - clf0[i+1]) /
                        (a[i] - a[i+1]);
                    break;
                case -1: // flaps down
                    cl = clfd[i] - (a[i] - angle) * (clfd[i] - clfd[i+1]) /
                        (a[i] - a[i+1]);
                    break;
                case 1: // flaps up
                    cl = clfu[i] - (a[i] - angle) * (clfu[i] - clfu[i+1]) /
                        (a[i] - a[i+1]);
                    break;
            }
            break;
        }
    }
    return cl;
}

float DragCoefficient(float angle, int flaps)
{
    float cdf0[9] = {0.01f, 0.0074f, 0.004f, 0.009f, 0.013f, 0.023f, 0.05f, 0.12f, 0.21f};
    float cdfd[9] = {0.0065f, 0.0043f, 0.0055f, 0.0153f, 0.0221f, 0.0391f, 0.1f, 0.195f, 0.3f};
    float cdfu[9] = {0.005f, 0.0043f, 0.0055f, 0.02601f, 0.03757f, 0.06647f, 0.13f, 0.18f, 0.25f};
    float a[9] = {-8.0f, -4.0f, 0.0f, 4.0f, 8.0f, 12.0f, 16.0f, 20.0f, 24.0f};
    float cd;
    int i;
    cd = 0.5;
    for (i=0; i<8; i++)
    {
        if( (a[i] <= angle) && (a[i+1] > angle) )
        {
            switch(flaps)
            {
                case 0:// flaps not deflected
                    cd = cdf0[i] - (a[i] - angle) * (cdf0[i] - cdf0[i+1]) /
                        (a[i] - a[i+1]);
                    break;
                case -1: // flaps down
                    cd = cdfd[i] - (a[i] - angle) * (cdfd[i] - cdfd[i+1]) /
                        (a[i] - a[i+1]);
                    break;
                case 1: // flaps up
                    cd = cdfu[i] - (a[i] - angle) * (cdfu[i] - cdfu[i+1]) /
                        (a[i] - a[i+1]);
                    break;
            }
            break;
        }
    }

    return cd;
}

float RudderLiftCoefficient(float angle)
{
    float clf0[7] = {0.16f, 0.456f, 0.736f, 0.968f, 1.144f, 1.12f, 0.8f};
    float a[7] = {0.0f, 4.0f, 8.0f, 12.0f, 16.0f, 20.0f, 24.0f};
    float cl;
    int i;
    float aa = (float) fabs(angle);
    cl = 0;
    for (i=0; i<8; i++)
    {
        if( (a[i] <= aa) && (a[i+1] > aa) )
        {
            cl = clf0[i] - (a[i] - aa) * (clf0[i] - clf0[i+1]) /
                (a[i] - a[i+1]);
            if (angle < 0) cl = -cl;
            break;
        }
    }
    return cl;
}
//------------------------------------------------------------------------//
// Given the attack angle, this function returns the proper drag coefficient
// for a symmetric (no camber) airfoil without flaps.
float RudderDragCoefficient(float angle)
{
    float cdf0[7] = {0.0032f, 0.0072f, 0.0104f, 0.0184f, 0.04f, 0.096f, 0.168f};
    float a[7] = {0.0f, 4.0f, 8.0f, 12.0f, 16.0f, 20.0f, 24.0f};
    float cd;
    int i;
    float aa = (float) fabs(angle);
    cd = 0.5;
    for (i=0; i<8; i++)
    {
        if( (a[i] <= aa) && (a[i+1] > aa) )
        {
            cd = cdf0[i] - (a[i] - aa) * (cdf0[i] - cdf0[i+1]) /
                (a[i] - a[i+1]);
            break;
        }
    }
    return cd;
}



//b plane3d.cpp:calc_airplane_loads
void Plane3D::calc_airplane_loads() {
    net_force = glm::dvec3(0,0,0);
    net_torque = glm::dvec3(0,0,0);

    glm::dvec3 fb(0), tb(0);

    glm::dvec3 thrust_force(0, 0, -thrust);
    
    for(int i = 0; i < 7; ++i) {
        auto& a = airfoils[i];
        if(i == 6) {
            double in = glm::radians(airfoils[i].incidence);
            double di = glm::radians(airfoils[i].dihedral);
            airfoils[i].normal = glm::normalize(glm::dvec3(
                cos(in)*sin(di),
                cos(in)*cos(di),
                -sin(in)
            ));
        }

        glm::dvec3 vtmp = glm::cross(ang_vel, a.pos);
        glm::dvec3 local_vel = glm::conjugate(orientation) * vel + vtmp;
        double local_speed = glm::length(local_vel);

        glm::dvec3 drag;
        if (local_speed > 1.)
            drag = -local_vel/local_speed;

        glm::dvec3 lift;
        lift = glm::cross(glm::cross(drag, a.normal), drag);
        double lift_length = glm::length(lift);
        if (lift_length > 0.01)
            lift = glm::normalize(lift);

        double tmp = glm::dot(drag, a.normal);
        if (tmp > 1) tmp = 1;
        if (tmp < -1) tmp = -1;
        double aoa = glm::degrees(asin(tmp));

        const double rho = 1.225;
        tmp = 0.5 * rho * local_speed * a.area;
        glm::dvec3 resultant;
        if (i==6) {
            resultant = (lift * (double)RudderLiftCoefficient(aoa) +
                         drag * (double)RudderDragCoefficient(aoa)) * tmp;
        } else if (i==4 || i==5) {
            resultant = (lift * (double)LiftCoefficient(aoa, a.iflap) +
                         drag * (double)DragCoefficient(aoa, a.iflap)) * tmp;
        } else {
            resultant = (lift * (double)LiftCoefficient(aoa, a.iflap) +
                         drag * (double)DragCoefficient(aoa, a.iflap)) * tmp;
        }


        fb += resultant;
        
        vtmp = glm::cross(a.pos, resultant);
        tb += vtmp;


        debug_arrows.push_back({
            glm::vec3(pos + rotate(orientation, a.pos)), 
            glm::vec3(rotate(orientation, resultant)*.05),
            glm::vec3(1,0,1)
        });
    }

    fb += thrust_force;
    debug_arrows.push_back({
        glm::vec3(pos), 
        glm::vec3(rotate(orientation, thrust_force)*.05),
        glm::vec3(1,0,1)
    });

    net_force = rotate(orientation, fb);
    net_force.y -= 9.8 * mass;

    net_torque = tb;
}










