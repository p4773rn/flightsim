#ifndef FRONTEND3D_H
#define FRONTEND3D_H
#include <SFML/Graphics.hpp>
#include <vector>
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
    Frontend3d(const glm::ivec2& screen_size);
    void update(const glm::dvec3& planePos, const glm::dquat& orientation);
    void draw(sf::RenderWindow& window, const Plane &plane, 
              std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> debugArrows);
    void keyInput();
    void mouseInput(sf::Window& window);
    void input(const sf::Event& event);

    void switch_arrows() { render_arrows = !render_arrows; }
    void switch_sun_cycle() { sun_cycle = !sun_cycle; clock.restart(); }

    const Terrain& get_terrain() const {return terrain;}
    Renderer& get_renderer() {return renderer;}
private:
    Renderer renderer;
    Camera camera;
    Model planeModel;
    Terrain terrain;
	Grid3d grid;
	Arrow arrow;
	HUD hud;
    glm::vec3 planePos;
    glm::dvec3 posOffset;
    glm::vec3 yawPitchRoll;
    bool is_first_person = false;
    bool is_cam_fixed = false;
    float cameraDistance = 50; // distance from plane
    bool render_arrows = true;

    glm::ivec2 screen_size;
    
    sf::Clock clock;
    bool sun_cycle = false;
    float time = 0;
};


#endif
