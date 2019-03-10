#ifndef CLASS_HUD
#define CLASS_HUD
#include <SFML/Graphics.hpp>
#include "../../plane.h"

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
