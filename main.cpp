#include <SFML/Graphics.hpp>
#include <iostream>
#include "plot.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "table.h"
#include "plane.h"

float getNextRandom(float prev) {
    return prev + ((rand() % 100) - 50);
}

int main()
{
    srand((int)time(NULL));

    sf::RenderWindow window(sf::VideoMode(800, 600), "FlightSim");
    const int FPS = 120;
    window.setFramerateLimit(FPS);
    sf::Clock clock;
    float lastUpdateTime = clock.getElapsedTime().asSeconds();
    sf::View view = window.getDefaultView();

    Plot p = Plot(600, 600);
    
    Plane plane = Plane();

    std::pair<float, float> point = {0, plane.getHeight()};
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
        std::cout<<point.first<<"; "<<point.second<<std::endl; 
        p.add(point);
        window.setView(view);

        window.clear();
        p.draw(window);

        window.display();
    }
}
