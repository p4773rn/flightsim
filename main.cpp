#include <SFML/Graphics.hpp>
#include <iostream>
#include "plot.h"
#include <cmath>
#include <cstdlib>

float getNextRandom(float prev) {
    return prev + ((rand() % 100) - 50);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "FlightSim");
    const int FPS = 30;
    window.setFramerateLimit(FPS);
    sf::Clock clock;
    sf::View view = window.getDefaultView();

    sf::CircleShape dot(2);
    dot.setPosition(200, 300);
    Plot p = Plot(600, 600);
    int i = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        float t = clock.getElapsedTime().asSeconds() * 1000.0f;
        std::pair<float, float> point;
        point.first = t;
        if (i != 1) {
            i = 1;
            point.second = 0;
        } else {
            point.second = getNextRandom(p.values.back().second);
        }
        p.add(point);
        window.setView(view);

        window.clear();
        p.draw(window);

        window.display();
    }

    return 0;
}
