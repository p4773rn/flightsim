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
    const int FPS = 10;

    srand((int)time(NULL));

    sf::RenderWindow window(sf::VideoMode(800, 600), "FlightSim");
    window.setFramerateLimit(FPS);

    sf::Clock clock;
    double lastUpdateTime = clock.getElapsedTime().asSeconds();

    sf::View view = window.getDefaultView();

    // Plane height plot
    Plot p = Plot(600, 600);
    
    Plane plane = Plane();

    // Point that will be displayed on plot
    // point.first = elapsed time
    // point.second = height of the plane
    std::pair<double, double> point = {0, plane.getHeight()};

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        plane.update(lastUpdateTime - clock.getElapsedTime().asSeconds());
        lastUpdateTime = clock.getElapsedTime().asSeconds();

        point.first = clock.getElapsedTime().asSeconds() * 1000;
        point.second = plane.getHeight();
        p.add(point);

        // std::cout<<point.first<<"; "<<point.second<<std::endl;

        window.setView(view);
        window.clear();
        p.draw(window);

        window.display();
    }
}
