#include <SFML/Graphics.hpp>
#include <iostream>
#include "plot.h"
#include <cmath>
#include <cstdlib>
#include "table.h"

float getNextRandom(float prev) {
    return prev + ((rand() % 100) - 50);
}

int main()
{
    // Warning: unlike Hans' code this doesn't convert degrees to radians
    Table table({
        { -4.0,  -0.14,   0.014,  0,   -0.036 },
        { -2.0,   0.02,   0.008,  0,   -0.052 },
        {  0.0,   0.14,   0.008,  0,   -0.090 },
        {  2.0,   0.32,   0.012,  0,   -0.130 },
        {  4.0,   0.46,   0.020,  0,   -0.160 },
        {  6.0,   0.60,   0.030,  0,   -0.200 },
        {  8.0,   0.76,   0.044,  0,   -0.240 },
        { 10.0,   0.90,   0.060,  0,   -0.280 },
        { 12.0,   1.04,   0.070,  0,   -0.310 },
        { 14.0,   1.16,   0.096,  0,   -0.330 },
        { 15.0,   1.22,   0.110,  0,   -0.340 },
        { 16.0,   1.16,   0.140,  0,   -0.350 },  
        { 18.0,   1.02,   0.210,  0,   -0.384 },
        { 20.0,   0.94,   0.260,  0,   -0.390 }
    });

    sf::RenderWindow window(sf::VideoMode(800, 600), "FlightSim");
    const int FPS = 30;
    window.setFramerateLimit(FPS);
    sf::Clock clock;
    sf::View view = window.getDefaultView();

    Plot p = Plot(600, 600);
    std::pair<float, float> point = {0, 0};
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        float t = clock.getElapsedTime().asSeconds() * 1000.0f;
        point.first = t;
        point.second = getNextRandom(point.second);
        
        p.add(point);
        window.setView(view);

        window.clear();
        p.draw(window);

        window.display();
    }
}
