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
    
    if (!attitudeIndicatorTexture.loadFromFile("assets/ai.png"))
        std::cerr << "Error: attitudeIndicatorTexture not loaded" << std::endl;
    if (!attitudeBackgroundTexture.loadFromFile("assets/attitude_indicator_background.png"))
        std::cerr << "Error: attitudeBackgroundTexture not loaded" << std::endl;
    attitudeIndicatorSprite.setTexture(attitudeIndicatorTexture);
    sf::Vector2f origin(ai_wh.x/2.0f, ai_wh.y/2.0f);
    attitudeIndicatorSprite.setOrigin(origin);
    attitudeBackgroundSprite.setTexture(attitudeBackgroundTexture);
    sf::Vector2f aSize(attitudeBackgroundTexture.getSize().x, attitudeBackgroundTexture.getSize().y);
    attitudeBackgroundSprite.setOrigin(sf::Vector2f(aSize.x/2.0f, aSize.y/2.0f));
}


void HUD::draw(sf::RenderWindow& window, const Plane &plane){
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//window.resetGLStates();
	window.pushGLStates();
	std::ostringstream sout;
	sout.precision(2);
	sout.flags(std::ios_base::fixed);
    sout << "Z Distance: " << -plane.getPos().x + 1000 << std::endl;
	sout << "Altitude: " << plane.getPos().y + 1728 << std::endl;
	sout << "Velocity: " << glm::length(plane.getVelocity()) << std::endl;
	sout << "Vertical Velocity: " << plane.getVelocity().y << std::endl;
	sout << "Throttle: " << plane.getThrottle() << std::endl;
	sout << "Elevator: " << plane.getElevatorDeflection() * 100 << std::endl;
	sout << "Flaps: " << plane.getFlaps() * 100 << std::endl;
	sout << "Brake: " << (plane.getBrakesStatus() ? "Yes" : "No") << std::endl;
	//sout << "Wings AoA: " << plane.getWingsAoA() << " degrees" << std::endl;
	//sout << "Elevators AoA: " << plane.getElevatorsAoA() << " degrees" << std::endl;
	sout << "Pitch Angle: " << plane.getPitchAngle() << std::endl;
	sout << "Roll Angle: " << plane.getAngles().z * 180 / M_PI << std::endl;
	sout << "Yaw Angle: " << plane.getYawAngle() << std::endl;
	text.setString(sout.str());
	text.setCharacterSize(12);
	//text.setRotation(-45.0);
	window.draw(text);
	//
	sf::Vector2u size = window.getSize();
	sf::IntRect rect = speedometerSprite.getTextureRect();
    sf::Vector2f pos(rect.width/2.0f, size.y - rect.height/2.0f);//rect.width, rect.height);
    speedometerSprite.setPosition(pos);
    pointerSprite.setPosition(pos);
    float maxSpeed = 200;
    pointerSprite.setRotation(glm::length(plane.getVelocity())/maxSpeed * 360);
    window.draw(speedometerSprite);
    window.draw(pointerSprite);

	//// Attitude Indicator
    pos = sf::Vector2f(size.x - ai_wh.x/2.0f, size.y - ai_wh.y/2.0f);
    //// 10 degrees map to 30 pixels on indicator sprite
    //float y_offset = -plane.getAngles().x * 180.0f / M_PI * 3;
    float y_offset = -plane.getPitchAngle() * 3;
    attitudeIndicatorSprite.setPosition(pos);// + sf::Vector2f(0, yOffset));
    ////clip texture
    attitudeIndicatorSprite.setTextureRect(sf::IntRect(0, ai_position.y + y_offset, ai_wh.x, ai_wh.y));
    attitudeBackgroundSprite.setPosition(pos);
    window.draw(attitudeIndicatorSprite);
    window.draw(attitudeBackgroundSprite);
	window.popGLStates();
}

