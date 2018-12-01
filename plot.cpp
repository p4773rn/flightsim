#include "plot.h"
#include <iostream>
#include <vector>
#include <cmath>

Plot::Plot(unsigned int _width, unsigned int _height)
    : width(_width), height(_height) 
{
    x = 0;
    y = 0;
    interval = 2000.0f;
    maxY = 1000;
    vertices = sf::VertexArray(sf::LineStrip, 200);

    if (!texture.loadFromFile("openGL/assets/plane.png"))
        std::cerr << "Error: plane texture not loaded" << std::endl;
    texture.setSmooth(true);
    sprite.setTexture(texture);
    sprite.scale(sf::Vector2f(0.1, 0.1));

    sf::Vector2f size(texture.getSize().x, texture.getSize().y);
    sprite.setOrigin(size/2.0f);
}

void Plot::add(std::pair<double, double> value) {
    if (std::abs(value.second) >= maxY)
        maxY = std::abs(value.second);
    
    double t = value.first - interval;
    while(!values.empty() && values.front().first < t) {
        values.erase(values.begin());
    }
    values.push_back(value);
}

void Plot::draw(sf::RenderWindow& window, float angle) {
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
        vertices[i].position.x = pX * 0.8;
        vertices[i].position.y = pY * 0.8;
        vertices[i].color = sf::Color::Red;
    }
    window.draw(box);
    window.draw(vertices);


    sprite.setPosition(vertices[vertices.getVertexCount()-1].position);
    sprite.setRotation(-angle / M_PI * 180);
    window.draw(sprite);
}
