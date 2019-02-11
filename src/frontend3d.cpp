#include "frontend3d.h"
#include <GL/glew.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

Frontend3d::Frontend3d() :
    camera(glm::vec3(0.0, 40.0f, 0.0))

{
    if (GLEW_OK != glewInit()) std::cerr << "GLEW INIT Error";
    
    std::vector<std::pair<std::string, GLuint>> paths = {
      std::pair<std::string, GLuint>("src/openGL/shaders/basic.vrtx", GL_VERTEX_SHADER),
      std::pair<std::string, GLuint>("src/openGL/shaders/basic.frgmnt", GL_FRAGMENT_SHADER)
    };
    mainShader = std::make_unique<Shader>(paths);
   
    planeModel = std::make_unique<Model>("assets/models/boeing.obj");
    sky = std::make_unique<Sky>("assets/terrain/textures/sky");
    terrain = std::make_unique<Terrain>("assets/terrain/hm.png", camera.get_position());

    grid = std::make_unique<Grid3d>();
}

void Frontend3d::update(const glm::vec3& planePos, const glm::vec3& yawPitchRoll) {
    this->planePos = planePos;
    this->yawPitchRoll = yawPitchRoll;
}

void Frontend3d::draw(sf::RenderWindow& window) {
    float yaw = yawPitchRoll.x;
    float pitch = yawPitchRoll.y;
    float roll = yawPitchRoll.z;
    glm::mat4 planeTransform =  glm::translate(glm::mat4(1), planePos) * glm::yawPitchRoll(yaw, pitch, roll);

    
    glm::mat4 view = camera.get_view();
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 800.0f/600.0f, 0.1f, 4096.0f);
    glm::vec4 light(-500.0f, 800.0f, 2048.0f, 1.0f);
  
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	sky->render(camera.get_view_no_translate(), projection);
	grid->render(planePos, camera.get_view(), projection);
    //terrain->draw(camera.get_position(), view, projection, glm::vec3(light));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    planeModel->draw(glm::scale(planeTransform, glm::vec3(10,10,10)), view, projection, 
                camera.get_position(), glm::vec3(view * light));
    glDisable(GL_BLEND);
}


void Frontend3d::keyInput() {
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
//      camera.move_position(0);
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
//      camera.move_position(1);
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
//      camera.move_position(2);
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
//      camera.move_position(3);
}


void Frontend3d::mouseInput(sf::Window& window){
    int last_x = 400;
    int last_y = 300;

    sf::Vector2i pos = sf::Mouse::getPosition(window);
    float offset_x = (pos.x - last_x)/(float)window.getSize().x;
    float offset_y = (last_y - pos.y)/(float)window.getSize().y;
    sf::Mouse::setPosition(sf::Vector2i(last_x, last_y), window);
    
    
    //camera.move_mouse(offset_x, offset_y);
    cameraDistance = clamp(cameraDistance, 100.0f, 1500.0f);
    camera.orbit(offset_x, offset_y, cameraDistance, planePos); 
}

void Frontend3d::input(const sf::Event& event) {
    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
        cameraDistance += event.mouseWheelScroll.delta * -20;
}
