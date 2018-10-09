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
    const int FPS = 60;

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
            if (event.type == sf::Event::Closed)
                window.close();
        }

        plane.update((clock.getElapsedTime().asSeconds() - lastUpdateTime) * 2);
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
