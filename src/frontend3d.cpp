#include "frontend3d.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <cmath>

const glm::vec3 pilot_default_offset(0.0, 4.0f, 19.0f);

Frontend3d::Frontend3d():
		camera(glm::vec3(0.0, 40.0f, 0.0)),
		mainShader({{"src/openGL/shaders/basic.vrtx", GL_VERTEX_SHADER},
					{"src/openGL/shaders/basic.frgmnt", GL_FRAGMENT_SHADER}}),
		//planeModel("assets/models/737-300.ac"),
		planeModel("assets/models/747-400.ac"), //747 is lighter model, might be useful for acceleration purposes
		sky("assets/terrain/textures/sky")
{	
	planeModel.set_position(glm::vec3(0.0f, planePos.y, -planePos.z));
	planeModel.set_default_rotation(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));
	std::cout << "Frontend3D was created...\n";
};

void Frontend3d::update(const glm::vec3& planePos, const glm::vec3& yawPitchRoll) {
    this->planePos = planePos;
    this->yawPitchRoll = yawPitchRoll;
    
    planeModel.set_position(planePos + glm::vec3(0.0f, 4.5f, 0.0f)); //For the 747-400
    //planeModel.set_position(planePos + glm::vec3(0.0f, 1.5f, 0.0f));
    planeModel.set_rotation(this->yawPitchRoll);
    if (is_first_person) {
    	glm::vec3 pilot_offset(0.0f,
    		pilot_default_offset.z * sin(yawPitchRoll.y) + pilot_default_offset.y * cos(yawPitchRoll.y),
    		-pilot_default_offset.z * cos(yawPitchRoll.y) + pilot_default_offset.y * sin(yawPitchRoll.y));
    	camera.set_position(this->planePos + glm::vec3(0.0f, 4.5f, 0.0f) + pilot_offset);
    }
}

void Frontend3d::draw(sf::RenderWindow& window, const Plane &plane) {
    float yaw = yawPitchRoll.x;
    float pitch = yawPitchRoll.y;
    float roll = yawPitchRoll.z;
    //glm::mat4 planeTransform = glm::translate(glm::mat4(1), planePos) * glm::yawPitchRoll(yaw, pitch, roll);
    glm::mat4 view = camera.get_view();
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 800.0f/600.0f, 0.1f, 4*4096.0f);
    glm::vec3 light(-500.0f, 800.0f, 2048.0f);
  	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	//sky.render(camera.get_view_no_translate(), projection);
	//arrow.set_uniforms(view, projection, cameraDistance);
	//arrow.draw(planePos, glm::vec3(1.0, 1.0f, -1.0f), 50.0f);
	//arrow.draw(planePos + glm::vec3(0.0f, -1.5f, 25.0f), glm::vec3(0.0f, 0.0f, -1.0f), 42.0f);
	grid.render(planePos, view, projection);
    //terrain->draw(camera.get_position(), view, projection, glm::vec3(light));
    //DRAWING MODELS
  	mainShader.use();
  	glUniformMatrix4fv(mainShader.get_loc("view"), 1, GL_FALSE, glm::value_ptr(view));
  	glUniformMatrix4fv(mainShader.get_loc("projection"), 1, GL_FALSE, glm::value_ptr(projection));
  	glUniform3fv(mainShader.get_loc("light_source"), 1, glm::value_ptr(light));
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (!is_first_person) planeModel.draw(mainShader);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glDisable(GL_BLEND);
}


void Frontend3d::keyInput() {
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
//      camera.move_position(0);
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
//      camera.move_position(1);
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
//      camera.move_position(2);
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
//      camera.move_position(3);
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
		is_first_person = !is_first_person;
	if (is_first_person && sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		is_cam_fixed = !is_cam_fixed;*/
}


void Frontend3d::mouseInput(sf::Window& window){
    int last_x = 400;
    int last_y = 300;

    sf::Vector2i pos = sf::Mouse::getPosition(window);
    float offset_x = (pos.x - last_x)/(float)window.getSize().x;
    float offset_y = (last_y - pos.y)/(float)window.getSize().y;
    sf::Mouse::setPosition(sf::Vector2i(last_x, last_y), window);
    
    if (is_first_person) {
    	if (!is_cam_fixed) {
    		camera.move_mouse(offset_x, offset_y);
    	} else {
    		float alpha = yawPitchRoll.y;
    		camera.look_in(glm::vec3(0.0, sin(alpha), -cos(alpha)));
    	}
    } else {
    	cameraDistance = clamp(cameraDistance, 10.0f, 1500.0f);
    	camera.orbit(offset_x, offset_y, cameraDistance, planePos);
    }
}

void Frontend3d::input(const sf::Event& event) {
    if (!is_first_person && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
        cameraDistance += event.mouseWheelScroll.delta * -10;
	if (event.type == sf::Event::KeyPressed) {
		switch(event.key.code) {
			case sf::Keyboard::Key::V:
				is_first_person = !is_first_person;
				break;
			case sf::Keyboard::Key::F:
				is_cam_fixed = is_first_person ? !is_cam_fixed : is_cam_fixed;
				break;
		}
	}
}
