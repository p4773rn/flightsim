#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include "openGL/src/renderer.h"
#include "openGL/src/camera/camera.h"
#include "openGL/src/model/model.h"
#include "openGL/src/terrain/terrain.h"
#include "openGL/src/misc.h"

void keyInput(Camera& camera, float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
      camera.move_position(0, deltaTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
      camera.move_position(1, deltaTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
      camera.move_position(2, deltaTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
      camera.move_position(3, deltaTime);
}


void mouseInput(Camera& camera, sf::Window& window, float deltaTime){
    int last_x = 400;
    int last_y = 300;

    sf::Vector2i pos = sf::Mouse::getPosition(window);
    float offset_x = (pos.x - last_x)/(float)window.getSize().x;
    float offset_y = (last_y - pos.y)/(float)window.getSize().y;
    sf::Mouse::setPosition(sf::Vector2i(last_x, last_y), window);
    
    
    camera.move_mouse(offset_x, offset_y, deltaTime);
}


int main()
{
    const int FPS = 100; 
    
    sf::Clock clock;
    double lastUpdateTime = clock.getElapsedTime().asSeconds();
    
    
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
   
    
    const glm::ivec2 SCREEN_SIZE(800, 600); 
    sf::RenderWindow window(sf::VideoMode(SCREEN_SIZE.x, SCREEN_SIZE.y), "FlightSim", sf::Style::Default, settings);
    //window.setFramerateLimit(FPS);
    window.setMouseCursorVisible(false);

    
    if (GLEW_OK != glewInit()) std::cerr << "GLEW INIT Error";
    
    

    // renderer loses precision after 100K meters (try setting x-pos of camera,model,terrain to 2^20)
    // offset physics outputs to be not too far from origin
    Renderer renderer(SCREEN_SIZE);
    Camera camera(glm::vec3(0, 200.0f, 0.0));
    camera.move_mouse(0, -1000, 1);
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), float(SCREEN_SIZE.x)/SCREEN_SIZE.y, 0.1f, 16000.0f);
   
    glm::vec3 light_color(1, 1, 0.9);

    Model model("assets/models/BGEAR_plane.obj");
    glm::mat4 model_model = glm::rotate(glm::translate(glm::mat4(1), glm::vec3(0, 10, -6)), 1.0f, glm::vec3(0, 1, 0));
    
    Terrain terrain(glm::vec3(0, 0, 0), 20000);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }

        float deltaTime = clock.getElapsedTime().asSeconds() - lastUpdateTime;
        lastUpdateTime = clock.getElapsedTime().asSeconds();
        
        keyInput(camera, deltaTime);
        mouseInput(camera, window, deltaTime);

        terrain.set_origin(terrain.get_origin() + glm::vec3(1000, 0, 0)*deltaTime);
        window.clear();
    
        model_model = glm::rotate(model_model, .01f, glm::vec3(0, 1, 0));
        
        for (const glm::vec3& pos : terrain.get_forest_positions(camera.get_position())) {
            glm::mat4 m = glm::translate(glm::mat4(1), pos) * glm::scale(glm::mat4(1), glm::vec3(20));
            renderer.queue_render(&(model.get_meshes()[0]), m);
        }
        for (Mesh& mesh : model.get_meshes()) {
            renderer.queue_render(&mesh, model_model);
        }

        renderer.queue_render(&terrain, glm::mat4(0));
        //std::cerr << camera.get_position() << std::endl;
        float angle = clock.getElapsedTime().asSeconds() * 0.5;
        glm::vec3 light_dir(cos(angle)*0.5+0.5, sin(angle)*0.5+0.5, 0.3);
        renderer.render(light_dir, light_color, projection, camera.get_view(), camera.get_position());
        window.display();
    }

    return 0;
}
