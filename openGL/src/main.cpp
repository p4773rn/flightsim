#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cstdio>
#include <GL/glew.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "model/model.h"
#include "shaders/shader.h"
#include "camera/camera.h"
#include "core/quaternion.h"
#include "terrain/terrain.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//const float SENSITIVITY = 0.125;

Terrain* terrain;
Shader* mainShader;
Model* model;
Camera* camera;
int last_x = 400;
int last_y = 300;

void renderScene(sf::Window& win) {
  //glUniformMatrix4fv(glGetUniformLocation(mainShader->getID(), "model"),
  //                   1, GL_FALSE, glm::value_ptr(model->scale()));
  glm::mat4 view;
  view = camera->getView();
  glUniformMatrix4fv(glGetUniformLocation(mainShader->getID(), "view"),
                     1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(mainShader->getID(), "projection"),
                     1, GL_FALSE,
                     glm::value_ptr(glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f)));

  glm::vec4 light(1.0f, 1.0f, 1.0f, 1.0f);
  light = view * light;
  glUniform3fv(glGetUniformLocation(mainShader->getID(), "lightSource"), 1,
               glm::value_ptr(glm::vec3(light.x, light.y, light.z)));


  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //model->draw();
  terrain->draw(view, glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 10.0f, 2000.0f));
  win.display();
}

void keyInput() {
  float speed = 0.01;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    camera->movePosition(0);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    camera->movePosition(1);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    camera->movePosition(2);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    camera->movePosition(3);
}


void mouseInput(sf::Window& window){
  sf::Vector2i pos = sf::Mouse::getPosition(window);
  float offset_x = (pos.x - last_x)/(float)window.getSize().x;
  float offset_y = (last_y - pos.y)/(float)window.getSize().y;
  camera->moveMouse(offset_x, offset_y);
  sf::Mouse::setPosition(sf::Vector2i(last_x, last_y), window);
}


int main(int argc, char** argv) {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    sf::Window window(sf::VideoMode(800, 600), "OpenGL Demo", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    window.setActive(true);

    if (GLEW_OK != glewInit()) std::cerr << "GLEW INIT Error";


    std::vector<std::pair<std::string, GLuint>> paths = {
      std::pair<std::string, GLuint>("shaders/basic.vrtx", GL_VERTEX_SHADER),
      std::pair<std::string, GLuint>("shaders/basic.frgmnt", GL_FRAGMENT_SHADER)
    };
    mainShader = new Shader(paths);
    mainShader->use();


    camera = new Camera(glm::vec3(1.0f, 500.0f, 0.5f));
    //model = new Model("assets/models/" +  std::string((argc == 2) ? (argv[1]) : "tree.obj"));
    
    terrain = new Terrain("assets/terrain/hm.png");
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
        renderScene(window);
    }

    delete terrain;
    delete mainShader;
    delete model;
    delete camera;
    return 0;
}
