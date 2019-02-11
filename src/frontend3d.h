#ifndef FRONTEND3D_H
#define FRONTEND3D_H

#include <SFML/Graphics.hpp>
#include "plane.h"
#include <list>
#include "misc.h"
#include <memory>
#include "openGL/src/camera/camera.h"
#include "openGL/src/terrain/terrain.h"
#include "openGL/src/terrain/sky.h"
#include "openGL/src/model/model.h"
#include "openGL/src/shaders/shader.h"


class Frontend3d {
public:
    Frontend3d();

    void update(const Plane& plane);

    void draw(sf::RenderWindow& window);
    

private:
    Vec2 position;
    float pitch;

    float altitude;
    Vec2 velocity;
    int throttle;
    float elevatorDeflection;
    float flaps;
    bool brakes;

    Camera camera;
    std::unique_ptr<Shader> mainShader;
    std::unique_ptr<Model> planeModel;
    std::unique_ptr<Sky> sky;
    std::unique_ptr<Terrain> terrain;
};


#endif
