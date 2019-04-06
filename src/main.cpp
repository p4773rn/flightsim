#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "table.h"
#include "plane.h"
#include "frontend3d.h"
//#include "//frontend2d.h"
#include <glm/glm.hpp>
#include "plot.h"
#include "airfoil_segment.h"

int main()
{
    const int SPEED = 1; // simulation speed; 1 = real time; 10 = 10 times faster;
    const int FPS = 60; 
    
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    sf::RenderWindow window(sf::VideoMode(1920 * 3 / 4, 1080 * 3 / 4), "FlightSim", sf::Style::Default, settings);
    //sf::RenderWindow window(sf::VideoMode(800, 600), "FlightSim");
    window.setFramerateLimit(FPS);
    window.setMouseCursorVisible(false);

    int screenWidth = sf::VideoMode::getDesktopMode().width;
    if (screenWidth > 1920)
        screenWidth = 1920;
    sf::Vector2<int> windowPosition(1920 / 4, 0);

    window.setPosition(windowPosition);


    sf::View view = window.getDefaultView();

	//WE INIT GLEW BEFORE //frontend
	if (GLEW_OK != glewInit()) std::cerr << "GLEW INIT Error";

    Frontend3d frontend;
    std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> debugArrows;
    
    Plane plane = Plane::getDefaultPlane(debugArrows);
    // plane.setThrottle(75);



    // TODO: put this inside plane somewhere
    double elevatorsDevlectionStep = 0.02;
	
	sf::CircleShape circle;
	circle.setRadius(20.0f);
	circle.setPosition(20, 30);
    sf::Clock clock;
    double lastUpdateTime = clock.getElapsedTime().asSeconds();
    
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

                        case sf::Keyboard::W:
                            plane.setRudderDeflection(plane.getRudderDeflection() - elevatorsDevlectionStep);
                            break;
                        case sf::Keyboard::Q:
                            plane.setRudderDeflection(plane.getRudderDeflection() + elevatorsDevlectionStep);
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
                    }
                    break;

                default:
                    break;
            }
        }

        debugArrows.clear();
        
        //std::cout << clock.getElapsedTime().asSeconds() * SPEED << " " << std::endl;
        float delta = (clock.getElapsedTime().asSeconds() - lastUpdateTime) * SPEED;
        plane.update(delta, debugArrows);
        lastUpdateTime = clock.getElapsedTime().asSeconds();

        frontend.mouseInput(window);
        frontend.keyInput();

        // negative z axis is forward
        glm::vec3 planePos = plane.getPos();
        
        glm::vec3 euler = plane.getAngles();
        glm::vec3 yawPitchRoll(euler.y,euler.x,euler.z);
        frontend.update(planePos, yawPitchRoll);

        //window.clear(sf::Color(127, 142,123));
        frontend.draw(window, plane, debugArrows);


       
        //window.pushGLStates();
		//window.popGLStates();
        window.display();
    }

    return 0;
}
