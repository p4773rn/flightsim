#ifndef CLASS_HUD
#define CLASS_HUD
#include <SFML/Graphics.hpp>
#include "../../plane.h"
//Hardcoded offsets for attitudeindicator texture
const sf::Vector2i ai_position = {0,212}; //20 degrees starts at 212
const sf::Vector2i ai_wh = {143, 132}; //width of texture and height of 20 to -20 range

class HUD {
public:
	HUD();
	void draw(sf::RenderWindow& window, const Plane &plane);
private:
	sf::Font font;
	sf::Text text;

    sf::Texture speedometerTexture;
    sf::Texture pointerTexture;
    sf::Sprite speedometerSprite;
    sf::Sprite pointerSprite;

    sf::Texture attitudeIndicatorTexture;
    sf::Texture attitudeBackgroundTexture;
    sf::Sprite attitudeIndicatorSprite;
    sf::Sprite attitudeBackgroundSprite;
};

#endif
