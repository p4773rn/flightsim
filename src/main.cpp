#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "table.h"
#include "plane.h"
#include "frontend3d.h"
//#include "//frontend2d.h"
#include <glm/glm.hpp>
#include "plot.h"
#include "airfoil_segment.h"


int main()
{
    float speed = 1; // simulation speed; 1 = real time; 10 = 10 times faster;
    const int FPS = 60; 
    
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    glm::ivec2 screenSize(800, 600);
    sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "FlightSim", sf::Style::Default, settings);
    window.setFramerateLimit(FPS);
    window.setMouseCursorVisible(false);

    //int screenWidth = sf::VideoMode::getDesktopMode().width;
    //if (screenWidth > 1920)
    //    screenWidth = 1920;
    sf::Vector2<int> windowPosition(1920 / 4, 0);

    //window.setPosition(windowPosition);


    sf::View view = window.getDefaultView();

	//WE INIT GLEW BEFORE //frontend
	if (GLEW_OK != glewInit()) std::cerr << "GLEW INIT Error";

    Frontend3d frontend(screenSize);
    std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> debugArrows;
    
    Plane plane = Plane::getDefaultPlane(debugArrows, frontend.get_terrain());
    // plane.setThrottle(75);



    // TODO: put this inside plane somewhere
    double elevatorsDevlectionStep = 0.02;
	
	sf::CircleShape circle;
	circle.setRadius(20.0f);
	circle.setPosition(20, 30);
    sf::Clock clock;
    double lastUpdateTime = clock.getElapsedTime().asSeconds();
 

    int frame = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            frontend.input(event);
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::Z:
                            plane.setThrottle(plane.getThrottle() - 5);
                            break;
                        case sf::Keyboard::X:
                            plane.setThrottle(plane.getThrottle() + 5);
                            break;
                        case sf::Keyboard::Up:
                            plane.setElevatorDeflection(plane.getElevatorDeflection() - elevatorsDevlectionStep);
                            break;
                        case sf::Keyboard::Down:
                            plane.setElevatorDeflection(plane.getElevatorDeflection() + elevatorsDevlectionStep);
                            break;

                        case sf::Keyboard::E:
                            plane.setElevatorDeflection(0);
                            break;
                        case sf::Keyboard::F:
                            plane.setFlaps(0);
                            break;
                        
                        case sf::Keyboard::Right:
                            plane.setFlaps(plane.getFlaps() - 0.02);
                            break;
                        case sf::Keyboard::Left:
                            plane.setFlaps(plane.getFlaps() + 0.02);
                            break;
                        case sf::Keyboard::B:
                            plane.toggleBrakes();
                            break;

                        case sf::Keyboard::Add:
                            speed += 0.1;
                            break;
                        case sf::Keyboard::Subtract:
                            speed = glm::max(0.0f, speed-0.1f);
                            break;
                        case sf::Keyboard::Divide:
                            speed = 1;
                            break;
                    }
                    break;

                default:
                    break;
            }
        }
        
        //std::cout << clock.getElapsedTime().asSeconds() * speed << " " << std::endl;
        float delta = (clock.getElapsedTime().asSeconds() - lastUpdateTime) * speed;
        
        int num_steps = 20;
        for (int i = 0; i < num_steps; ++i) {
            debugArrows.clear();
            plane.update(delta/num_steps, debugArrows);
        }

        lastUpdateTime = clock.getElapsedTime().asSeconds();


        glm::dvec3 planePos = plane.getPos();
        //planePos.z = clock.getElapsedTime().asSeconds() * 1000;
        frontend.update(planePos, plane.getOrientation());
        frontend.mouseInput(window);
        frontend.keyInput();

        //window.clear(sf::Color(127, 142,123));
        frontend.draw(window, plane, debugArrows);
       
        //window.pushGLStates();
		//window.popGLStates();
        window.display();
    }

    return 0;
}
