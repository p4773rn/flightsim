#ifndef FRONTEND3D_H
#define FRONTEND3D_H
#include <SFML/Graphics.hpp>
#include <list>
#include <memory>
#include "plane.h"
#include "misc.h"
#include "openGL/camera/camera.h"
#include "openGL/terrain/terrain.h"
#include "openGL/model/model.h"
#include "openGL/shaders/shader.h"
#include "openGL/tools/grid3d.h"
#include "openGL/tools/arrow.h"
#include "openGL/tools/hud.h"
#include "openGL/renderer.h"

class Frontend3d {
public:
    Frontend3d ();
    void update(const glm::vec3& planePos, const glm::vec3& yawPitchRoll);
    void draw(sf::RenderWindow& window, const Plane &plane);
    void keyInput();
    void mouseInput(sf::Window& window);
    void input(const sf::Event& event);
private:
    Renderer renderer;
    Camera camera;
    Model planeModel;
    Terrain terrain;
	Grid3d grid;
	Arrow arrow;
	HUD hud;
    glm::vec3 planePos;
    glm::vec3 yawPitchRoll;
    bool is_first_person = false;
    bool is_cam_fixed = false;
    float cameraDistance = 50; // distance from plane
};


#endif
