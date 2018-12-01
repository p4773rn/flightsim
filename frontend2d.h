#ifndef FRONTEND2D_H
#define FRONTEND2D_H

#include <SFML/Graphics.hpp>
#include "plane.h"
#include <list>
#include "misc.h"

class Frontend2d {
public:
    Frontend2d();

    void update(const Plane& plane);
    void updateCamera(sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);
    void drawTrail(sf::RenderWindow& window);
    void drawGrid(sf::RenderWindow& window);
    void drawSprites(sf::RenderWindow& window);
    void drawGround(sf::RenderWindow& window);
    void drawSky(sf::RenderWindow& window);
    void drawHud(sf::RenderWindow& window);

    float getScale() const { return scale; }
    void setScale(float scale) { this->scale = clamp<float>(scale, 1, 10); }

private:
    std::list<sf::Vector2f> points;
    float pitch;

    float altitude;
    Vec2 velocity;
    int throttle;
    float elevatorDeflection;
    float flaps;
    bool brakes;

    float scale = 1;
    sf::Vector2f cameraPos; // meters
    sf::Vector2f cameraSize; // meters

    sf::Texture planeTexture;
    sf::Sprite planeSprite;

    sf::Texture cloudTexture;

    sf::Font font;
    sf::Text hudText;
    sf::View hudView;
};


#endif
