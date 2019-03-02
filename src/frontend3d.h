#ifndef FRONTEND3D_H
#define FRONTEND3D_H

#include <SFML/Graphics.hpp>
#include <list>
#include <memory>
#include "plane.h"
#include "misc.h"
#include "openGL/camera/camera.h"
#include "openGL/terrain/terrain.h"
#include "openGL/terrain/sky.h"
#include "openGL/model/model.h"
#include "openGL/shaders/shader.h"
#include "openGL/grid3d.h"
#include "openGL/arrow.h"


class Frontend3d {
public:
    Frontend3d () :
		camera(glm::vec3(0.0, 40.0f, 0.0)),
		mainShader({{"src/openGL/shaders/basic.vrtx", GL_VERTEX_SHADER},
					{"src/openGL/shaders/basic.frgmnt", GL_FRAGMENT_SHADER}}),
		planeModel("assets/models/boeing.obj"),
		sky("assets/terrain/textures/sky")
	{std::cout << "Frontend3D was created...\n";};

    void update(const glm::vec3& planePos, const glm::vec3& yawPitchRoll);

    void draw(sf::RenderWindow& window, const Plane &plane);
    
    void keyInput();
    void mouseInput(sf::Window& window);
    void input(const sf::Event& event);
    
private:
    glm::vec3 planePos;
    glm::vec3 yawPitchRoll;
    
    Camera camera;
    Shader mainShader;
    Model planeModel;
    Sky sky;
	Grid3d grid;
	Arrow arrow;
    float cameraDistance = 50; // distance from plane
    //Terrain terrain;
};


#endif
