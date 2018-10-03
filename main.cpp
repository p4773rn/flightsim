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

    srand((int)time(NULL));

    sf::RenderWindow window(sf::VideoMode(800, 600), "FlightSim");
    window.setFramerateLimit(FPS);

    sf::Clock clock;
    double lastUpdateTime = clock.getElapsedTime().asSeconds();

    sf::View view = window.getDefaultView();

    // Plane height plot
    Plot p1 = Plot(800, 200);
    Plot p2 = Plot(800, 200);
    Plot p3 = Plot(800, 200);
    p2.setPosition(0, 200);
    p3.setPosition(0, 400);
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
        plane.update((clock.getElapsedTime().asSeconds() - lastUpdateTime) / 1);
        lastUpdateTime = clock.getElapsedTime().asSeconds();

        // point.first = clock.getElapsedTime().asSeconds() * 1000;
        point.second = plane.getHeight();
        point.first = plane.getPos().getX();
        p1.add(point);
        p2.add(point);
        p3.add(point);

        // std::cout<<point.first<<"; "<<point.second<<std::endl;

        window.setView(view);
        window.clear();
        p1.draw(window);
        p2.draw(window);
        p3.draw(window);

        window.display();
    }
}
