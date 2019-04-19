#include "frontend3d.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <cmath>

const glm::vec3 pilot_default_offset(0.0, 0.0f, -6.0f);

Frontend3d::Frontend3d(const glm::ivec2& screen_size):
        renderer(screen_size),
		camera(glm::vec3(0.0, 40.0f, 0.0)),
		//planeModel("assets/models/737-400.ac"),
		planeModel("assets/models/737-300.ac"),
		//planeModel("assets/models/747-400.ac"), //747 is lighter model, might be useful for acceleration purposes
		terrain(glm::vec3(0, 0, 0), 1<<16),
		screen_size{screen_size}
{	
	planeModel.set_position(glm::vec3(0.0f, planePos.y, -planePos.z));
	planeModel.set_default_rotation(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));
	std::cout << "Frontend3D was created...\n";
};

void Frontend3d::update(const glm::dvec3& planePos, const glm::dquat& orientation) {
    posOffset = glm::floor(planePos / 100.0) * 100.0;
    this->planePos = planePos - posOffset;
	//(glm::mat4)glm::mat4_cast(orientation)
    //this->yawPitchRoll = yawPitchRoll;
    //planeModel.set_position(planePos + glm::vec3(0.0f, 4.5f, 0.0f)); //For the 747-400
    glm::vec3 forward = glm::quat(orientation) * glm::vec3(0,0,-1);
    glm::vec3 up = glm::quat(orientation) * glm::vec3(0,1,0);
    planeModel.set_position(this->planePos + forward * 5.0f + up * (-1.0f));
    planeModel.set_orientation(orientation);//this->yawPitchRoll);
    if (is_first_person) {
    	/*glm::vec3 pilot_offset(0.0f,
    		pilot_default_offset.z * sin(yawPitchRoll.y) + pilot_default_offset.y * cos(yawPitchRoll.y),
    		-pilot_default_offset.z * cos(yawPitchRoll.y) + pilot_default_offset.y * sin(yawPitchRoll.y));*/
    	glm::vec3 offset = (glm::mat3)glm::mat3_cast(orientation) * pilot_default_offset;
    	//camera.set_position(this->planePos + glm::vec3(0.0f, 4.5f, 0.0f) + pilot_offset);
    	camera.set_position(this->planePos + offset);
    }
}

void Frontend3d::draw(sf::RenderWindow& window, const Plane &plane, 
                      std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> debugArrows) {
    float yaw = yawPitchRoll.x;
    float pitch = yawPitchRoll.y;
    float roll = yawPitchRoll.z;
    //glm::mat4 planeTransform = glm::translate(glm::mat4(1), planePos) * glm::yawPitchRoll(yaw, pitch, roll);
    glm::mat4 view = camera.get_view();
    //glm::mat4 projection = glm::perspective(glm::radians(90.0f), 800.0f/600.0f, 0.1f, 4*4096.0f);
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), float(screen_size.x)/screen_size.y, 0.1f, 4*4096.0f);
    
    
    glm::vec3 light_dir;
    glm::vec3 light_color(1.0, 1.0, 1.0);

    if (sun_cycle) {
        time += clock.getElapsedTime().asSeconds();
        clock.restart();
    }
    float angle = glm::mod(time * 0.5 + 0.6, M_PI) + M_PI;
    light_dir = glm::normalize(glm::vec3(glm::cos(angle), glm::sin(angle), 0));


    //if (!is_first_person) 
        renderer.queue_render(&planeModel);

    //grid.set_pos(glm::vec3(planePos.x, 0, planePos.z));
    //renderer.queue_render(&grid);
    
    for (auto& a : debugArrows) {
        glm::vec3 origin, direction, color;
        std::tie(origin, direction, color) = a;
        origin -= posOffset;
        a = std::make_tuple(origin, direction, color);
    }
    if (render_arrows) {
        arrow.set_arrows(debugArrows, cameraDistance);
        renderer.queue_render(&arrow);
    }


    terrain.set_pos_offset(posOffset);
    renderer.queue_render(&terrain);

    renderer.get_clouds().set_pos_offset(posOffset);
    renderer.render(light_dir, light_color, projection, view, camera.get_position());
    //hud.draw(window, plane);
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
    		//camera.look_in(glm::vec3(0.0, sin(alpha), -cos(alpha)));
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
			case sf::Keyboard::Key::L:
				is_cam_fixed = is_first_person ? !is_cam_fixed : is_cam_fixed;
				break;
		}
	}
}
