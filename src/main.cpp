#include <SFML/Graphics.hpp>
#include <iostream>
#include "plot.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "table.h"
#include "plane.h"
#include "frontend3d.h"
#include <glm/glm.hpp>

int main()
{
    const int SPEED = 3; // simulation speed; 1 = real time; 10 = 10 times faster;
    const int FPS = 60; 
    
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    sf::RenderWindow window(sf::VideoMode(800, 600), "FlightSim", sf::Style::Default, settings);
    window.setFramerateLimit(FPS);
    window.setMouseCursorVisible(false);

    sf::Clock clock;

    sf::View view = window.getDefaultView();
    std::cout << "TEST" << std::endl;

	//WE INIT GLEW BEFORE FRONTEND
	if (GLEW_OK != glewInit()) std::cerr << "GLEW INIT Error";
	//Model test("assets/models/boeing747-400-jw.ac");
	//return 0;
    Frontend3d frontend;
    Plane plane = Plane::getDefaultPlane();
    // plane.setThrottle(75);
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
                        case sf::Keyboard::Q:
                            plane.setElevatorDeflection(plane.getElevatorDeflection() - 0.1);
                            break;
                        case sf::Keyboard::W:
                            plane.setElevatorDeflection(plane.getElevatorDeflection() + 0.1);
                            break;
                        case sf::Keyboard::F:
                            plane.setFlaps(plane.getFlaps() - 0.25);
                            break;
                        case sf::Keyboard::G:
                            plane.setFlaps(plane.getFlaps() + 0.25);
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

        //std::cout << clock.getElapsedTime().asSeconds() * SPEED << " " << std::endl;

        plane.update((clock.getElapsedTime().asSeconds() - lastUpdateTime) * SPEED);
        lastUpdateTime = clock.getElapsedTime().asSeconds();

        frontend.mouseInput(window);
        frontend.keyInput();

        // negative z axis is forward
        glm::vec3 planePos(0, plane.getPos().getY(), -plane.getPos().getX());
        glm::vec3 yawPitchRoll(0, plane.getAngle(), 0);
        frontend.update(planePos, yawPitchRoll);

        window.clear();
        frontend.draw(window);


        window.display();
    }

    return 0;
}
