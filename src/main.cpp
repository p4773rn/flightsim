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
    const int SPEED = 2; // simulation speed; 1 = real time; 10 = 10 times faster;
    const int FPS = 60; 
    
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    sf::RenderWindow window(sf::VideoMode(800, 600), "FlightSim", sf::Style::Default, settings);
    window.setFramerateLimit(FPS);
    window.setMouseCursorVisible(false);

    int screenWidth = sf::VideoMode::getDesktopMode().width;
    if (screenWidth > 1920)
        screenWidth = 1920;
    sf::Vector2<int> windowPosition(screenWidth - 800, 0);

    window.setPosition(windowPosition);


    sf::View view = window.getDefaultView();
    
    Frontend3d frontend;

    Plane plane = Plane::getDefaultPlane();
    // plane.setThrottle(75);
    // TODO: put this inside plane somewhere
    double elevatorsDevlectionStep = 0.01;
    
    
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
                        case sf::Keyboard::Q:
                            plane.setElevatorDeflection(plane.getElevatorDeflection() - elevatorsDevlectionStep);
                            break;
                        case sf::Keyboard::W:
                            plane.setElevatorDeflection(plane.getElevatorDeflection() + elevatorsDevlectionStep);
                            break;
                        case sf::Keyboard::E:
                            plane.setElevatorDeflection(0);
                            break;
                        case sf::Keyboard::F:
                            plane.setFlaps(plane.getFlaps() - 0.05);
                            break;
                        case sf::Keyboard::G:
                            plane.setFlaps(plane.getFlaps() + 0.05);
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

        std::cout << clock.getElapsedTime().asSeconds() * SPEED << " " << std::endl;

        plane.update((clock.getElapsedTime().asSeconds() - lastUpdateTime) * SPEED);
        lastUpdateTime = clock.getElapsedTime().asSeconds();

        frontend.mouseInput(window);
        frontend.keyInput();

        // negative z axis is forward
        glm::vec3 planePos(0, plane.getPos().getY(), -plane.getPos().getX());
        glm::vec3 yawPitchRoll(0, plane.getPitchAngle(), 0);
        frontend.update(planePos, yawPitchRoll);

        window.clear();
        frontend.draw(window);


        window.display();
    }

    return 0;
}
