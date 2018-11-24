#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cstdio>
#include <GL/glew.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "model/model.h"
#include "shaders/shader.h"
#include "camera/camera.h"
#include "core/quaternion.h"
#include "terrain/terrain.h"
#include "terrain/sky.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//const float SENSITIVITY = 0.125;

Terrain* terrain;
Sky* sky;
Shader* mainShader;
Model* model;
Camera* camera;
int last_x = 400;
int last_y = 300;

glm::mat4 modelt;

void renderScene(sf::Window& win) {
    glm::mat4 view = camera->get_view();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 4096.0f);
    glm::vec4 light(800.0f, 800.0f, 1.0f, 1.0f);
    light = view * light;
  
  
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	sky->render(camera->get_view_no_translate(), projection);
    model->draw(glm::scale(modelt, glm::vec3(10,10,10)), view, projection, glm::vec3(light.x, light.y, light.z));
    terrain->draw(camera->get_position(), view, projection);
    
    win.display();
}

void keyInput() {
    float speed = 0.01;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
      camera->move_position(0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
      camera->move_position(1);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
      camera->move_position(2);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
      camera->move_position(3);
}


void mouseInput(sf::Window& window){
    sf::Vector2i pos = sf::Mouse::getPosition(window);
    float offset_x = (pos.x - last_x)/(float)window.getSize().x;
    float offset_y = (last_y - pos.y)/(float)window.getSize().y;
    camera->move_mouse(offset_x, offset_y);
    sf::Mouse::setPosition(sf::Vector2i(last_x, last_y), window);
}


int main(int argc, char** argv) {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL Demo", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    window.setActive(true);

    if (GLEW_OK != glewInit()) std::cerr << "GLEW INIT Error";


    std::vector<std::pair<std::string, GLuint>> paths = {
      std::pair<std::string, GLuint>("shaders/basic.vrtx", GL_VERTEX_SHADER),
      std::pair<std::string, GLuint>("shaders/basic.frgmnt", GL_FRAGMENT_SHADER)
    };
    mainShader = new Shader(paths);
    mainShader->use();


    camera = new Camera(glm::vec3(0.0, 200.0f, 0.0));

    model = new Model("assets/models/tree.obj");
  
    modelt = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -1024.0));

    terrain = new Terrain("assets/terrain/hm.png", camera->get_position());

	sky = new Sky("assets/terrain/textures/sky");

    sf::Clock clock;
    bool running = true;
    while(running) {
        sf::Event event;
        while(window.pollEvent(event)) {
            switch (event.type){
                case sf::Event::Closed:
                    running = false;
                    break;
            }
        }
        keyInput();
        mouseInput(window);
        mainShader->use();
        renderScene(window);
        int fps = int(1.0f/clock.getElapsedTime().asSeconds());
        //window.draw(fpsCounter);
		/*std::cout << "\r" << camera->get_position().x 
				  << ", " << camera->get_position().y
				  << ", " << camera->get_position().z << std::flush;*/
        window.display();
        clock.restart().asSeconds();
    }

    delete terrain;
    delete mainShader;
    delete model;
    delete camera;
    return 0;
}
