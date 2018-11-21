#ifndef FRONTEND2D_H
#define FRONTEND2D_H

#include <SFML/Graphics.hpp>
#include "plane.h"
#include <list>

class Frontend2d {
public:

    void update(const Plane& plane);
    void updateCamera(sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);
    void drawTrail(sf::RenderWindow& window);
    void drawGrid(sf::RenderWindow& window);

    double getScale() const { return scale; }
    void setScale(double scale) { this->scale = scale; }

private:
    int width, height;


    std::list<sf::Vector2f> positions;

    double scale = 1;
    sf::Vector2f cameraPos; // meters
    sf::Vector2f cameraSize; // meters
};


#endif
