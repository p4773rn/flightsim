#include "plot.h"

#include <iostream>

Plot::Plot(unsigned int _width, unsigned int _height)
    : width(_width), height(_height) 
{
    x = 0;
    y = 0;
    interval = 2000.0f;
    maxY = 0;
}

void Plot::add(std::pair<double, double> value) {
    if (std::abs(value.second) > maxY)
        maxY = std::abs(value.second);
    
    double t = value.first - interval;
    while(!values.empty() && values.front().first < t) {
        values.pop();
    }
    values.push(value);
}

void Plot::draw(sf::RenderWindow& window) {
    int iterations = values.size();
    double t = values.back().first; // time of newest value
    sf::CircleShape dot(1);
    for (unsigned int i = iterations; i > 0; i--) {
        std::pair<double, double> value = values.front(); // the oldest value in queue
        values.pop();
        unsigned int pointX, pointY; // points where to draw
        int offset = (1.0f - ((t - value.first) / interval)) * width;
        if (offset >= 0) {
            pointX = x + offset;
            pointY = y + height/2 - (height/2 * (value.second/maxY)); // height * (value.second/maxY) <= height/2
        }
        // std::cout<<maxY<<std::endl;
        dot.setPosition(pointX, pointY);
        window.draw(dot);
        values.push(value);
    }
}
