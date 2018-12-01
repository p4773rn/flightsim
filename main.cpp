#include <SFML/Graphics.hpp>
#include <iostream>
#include "plot.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "table.h"
#include "plane.h"
#include "frontend2d.h"

int main()
{
    const int SPEED = 1; // simulation speed; 1 = real time; 10 = 10 times faster;
    const int FPS = 30; 

    sf::RenderWindow window(sf::VideoMode(800, 600), "FlightSim");
    window.setFramerateLimit(FPS);

    sf::Clock clock;
    double lastUpdateTime = clock.getElapsedTime().asSeconds();

    sf::View view = window.getDefaultView();
    
    // Plane height plot
    Frontend2d frontend;

    Plane plane = Plane::getDefaultPlane();
    // plane.setThrottle(75);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
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
                        case sf::Keyboard::Add:
                            frontend.setScale(frontend.getScale() / 2);
                            break;
                        case sf::Keyboard::Subtract:
                            frontend.setScale(frontend.getScale() * 2);
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

        frontend.update(plane);

        window.clear();
        frontend.updateCamera(window);
        frontend.draw(window);


        window.display();
    }

    return 0;
}
