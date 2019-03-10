#include "hud.h"
#include <sstream>
#include <iostream>
#include <GL/glew.h>

HUD::HUD() {
    if(!font.loadFromFile("assets/DejaVuSansMono.ttf")) {
        std::cerr << "ERROR: font was not loaded" << std::endl;
    }
    text.setFont(font);
    if (!speedometerTexture.loadFromFile("assets/speedometer.png"))
        std::cerr << "Error: speedometerTexture not loaded" << std::endl;
    if (!pointerTexture.loadFromFile("assets/pointer.png"))
        std::cerr << "Error: pointerTexture not loaded" << std::endl;
    speedometerSprite.setTexture(speedometerTexture);
    sf::Vector2f sSize(speedometerTexture.getSize().x, speedometerTexture.getSize().y);
    speedometerSprite.setOrigin(sSize/2.0f);
    pointerSprite.setTexture(pointerTexture);
    pointerSprite.setOrigin(sSize/2.0f);
    
    if (!attitudeIndicatorTexture.loadFromFile("assets/attitude_indicator.png"))
        std::cerr << "Error: attitudeIndicatorTexture not loaded" << std::endl;
    if (!attitudeBackgroundTexture.loadFromFile("assets/attitude_indicator_background.png"))
        std::cerr << "Error: attitudeBackgroundTexture not loaded" << std::endl;
    attitudeIndicatorSprite.setTexture(attitudeIndicatorTexture);
    sf::Vector2f aSize(attitudeIndicatorTexture.getSize().x, attitudeIndicatorTexture.getSize().y);
    attitudeIndicatorSprite.setOrigin(aSize/2.0f);
    attitudeBackgroundSprite.setTexture(attitudeBackgroundTexture);
    attitudeBackgroundSprite.setOrigin(aSize/2.0f);
}

void HUD::draw(sf::RenderWindow& window, const Plane &plane){
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	window.resetGLStates();
	std::ostringstream sout;
	sout.precision(2);
	sout.flags(std::ios_base::fixed);
	sout << "Altitude: " << plane.getPos().getY() << std::endl;
	sout << "Velocity: " << plane.getVelocity().getX() << std::endl;
	sout << "Vertical Velocity: " << plane.getVelocity().getY() << std::endl;
	sout << "Throttle: " << plane.getThrottle() << std::endl;
	sout << "Elevator: " << plane.getElevatorDeflection() * 100 << std::endl;
	sout << "Flaps: " << plane.getFlaps() * 100 << std::endl;
	sout << "Brake: " << (plane.getBrakesStatus() ? "Yes" : "No") << std::endl;
	sout << "Wings AoA: " << plane.getWingsAoA() << " degrees" << std::endl;
	sout << "Elevators AoA: " << plane.getElevatorsAoA() << " degrees" << std::endl;
	text.setString(sout.str());
	text.setCharacterSize(12);
	//text.setRotation(-45.0);
	window.draw(text);
	
	sf::Vector2u size = window.getSize();
	sf::IntRect rect = speedometerSprite.getTextureRect();
    sf::Vector2f pos(rect.width/2.0f, size.y - rect.height/2.0f);//rect.width, rect.height);
    speedometerSprite.setPosition(pos);
    pointerSprite.setPosition(pos);
    float maxSpeed = 200;
    pointerSprite.setRotation(plane.getVelocity().length()/maxSpeed * 360);
    window.draw(speedometerSprite);
    window.draw(pointerSprite);

    rect = attitudeIndicatorSprite.getTextureRect();
    pos = sf::Vector2f(size.x - rect.width/2, size.y - rect.height/2);//pos.x + rect.width, rect.height);
    // 10 degrees map to 30 pixels on indicator sprite
    float yOffset = plane.getPitchAngle() * 180.0f / M_PI * 3;
    attitudeIndicatorSprite.setPosition(pos + sf::Vector2f(0, yOffset));
    attitudeBackgroundSprite.setPosition(pos);
    window.draw(attitudeIndicatorSprite);
    window.draw(attitudeBackgroundSprite);
}
