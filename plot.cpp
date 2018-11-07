#include "plot.h"
#include <iostream>
#include <vector>

Plot::Plot(unsigned int _width, unsigned int _height)
    : width(_width), height(_height) 
{
    x = 0;
    y = 0;
    interval = 2000.0f;
    maxY = 5000;
    vertices = sf::VertexArray(sf::LineStrip, 200);
}

void Plot::add(std::pair<double, double> value) {
    
    double t = value.first - interval;
    while(!values.empty() && values.front().first < t) {
        values.erase(values.begin());
    }
    values.push_back(value);
}

void Plot::draw(sf::RenderWindow& window) {
    sf::RectangleShape box(sf::Vector2f(width, height));
    box.setPosition(sf::Vector2f(x, y));
    box.setFillColor(sf::Color::Blue);

    if (vertices.getVertexCount() != values.size())
            vertices.resize(values.size());
    double t = values.back().first;
    for (unsigned int i = 0; i < values.size(); ++i) {
        unsigned int pX, pY;
        int offset = (1.0f - ((t - values[i].first) / interval)) * width;
        if (offset >= 0) {
            pX = x + offset;
            pY = y + height - (height * (values[i].second/maxY));
        }
        //vertices[i] = sf::Vertex(sf::Vector2f(pX, pY), sf::Color::Red);
        vertices[i].position.x = pX;
        vertices[i].position.y = pY;
        vertices[i].color = sf::Color::Red;
    }
    window.draw(box);
    window.draw(vertices);
}
