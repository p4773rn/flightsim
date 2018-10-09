#include <SFML/Graphics.hpp>
#include <iostream>
#include "plot.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "table.h"
#include "plane.h"

int main()
{
    const int SPEED = 20; // simulation speed; 1 = real time; 10 = 10 times faster;
    const int FPS = 30 * SPEED; 

    sf::RenderWindow window(sf::VideoMode(800, 600), "FlightSim");
    window.setFramerateLimit(FPS);

    sf::Clock clock;
    double lastUpdateTime = clock.getElapsedTime().asSeconds();

    sf::View view = window.getDefaultView();
    
    // Plane height plot
    Plot plot = Plot(800, 600);
    

    Plane plane = Plane::getDefaultPlane();

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
                    if (event.key.code == sf::Keyboard::Z)
                        std::cout << "throttle-" << std::endl;
                    if (event.key.code == sf::Keyboard::X)
                        std::cout << "throttle+" << std::endl;
                    break;

                default:
                    break;
            }
        }

        plane.update((clock.getElapsedTime().asSeconds() - lastUpdateTime) * SPEED);
        lastUpdateTime = clock.getElapsedTime().asSeconds();

        std::pair<double, double> point = {plane.getPos().getX(), plane.getPos().getY()};
        plot.add(point);

        window.setView(view);
        window.clear();
        plot.draw(window);

        window.display();
    }

    return 0;
}
