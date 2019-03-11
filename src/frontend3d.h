#ifndef FRONTEND3D_H
#define FRONTEND3D_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <tuple>
#include <memory>
#include "plane.h"
#include "misc.h"
#include "openGL/camera/camera.h"
#include "openGL/terrain/terrain.h"
#include "openGL/terrain/sky.h"
#include "openGL/model/model.h"
#include "openGL/shaders/shader.h"
#include "openGL/tools/grid3d.h"
#include "openGL/tools/arrow.h"

const unsigned int MAX_CAMERA = 2;

class Frontend3d {
public:
    Frontend3d ();
    void update(const glm::vec3& planePos, const glm::vec3& yawPitchRoll);
    void draw(sf::RenderWindow& window, const std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> arrows);
    void keyInput();
    void mouseInput(sf::Window& window);
    void input(const sf::Event& event);
private:
    Camera camera;
    Shader mainShader;
    Model planeModel;
    Sky sky;
	Grid3d grid;
	Arrow arrow;
    glm::vec3 planePos;
    glm::vec3 yawPitchRoll;
    bool is_first_person = false;
    bool is_cam_fixed = false;
    float cameraDistance = 50; // distance from plane
   
    //Terrain terrain;
};


#endif
