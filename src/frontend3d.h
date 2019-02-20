#ifndef FRONTEND3D_H
#define FRONTEND3D_H

#include <SFML/Graphics.hpp>
#include "plane.h"
#include <list>
#include "misc.h"
#include <memory>
#include "openGL/camera/camera.h"
#include "openGL/terrain/terrain.h"
#include "openGL/terrain/sky.h"
#include "openGL/model/model.h"
#include "openGL/shaders/shader.h"
#include "openGL/grid3d.h"


class Frontend3d {
public:
    Frontend3d();

    void update(const glm::vec3& planePos, const glm::vec3& yawPitchRoll);

    void draw(sf::RenderWindow& window);
    
    void keyInput();
    void mouseInput(sf::Window& window);
    void input(const sf::Event& event);
    
private:
    glm::vec3 planePos;
    glm::vec3 yawPitchRoll;
    
    Camera camera;
    float cameraDistance = 200; // distance from plane
    std::unique_ptr<Shader> mainShader;
    std::unique_ptr<Model> planeModel;
    std::unique_ptr<Sky> sky;
    std::unique_ptr<Terrain> terrain;

    std::unique_ptr<Grid3d> grid;
};


#endif
