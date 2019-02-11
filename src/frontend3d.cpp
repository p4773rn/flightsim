#include "frontend3d.h"
#include <GL/glew.h>

Frontend3d::Frontend3d() :
    camera(glm::vec3(0.0, 200.0f, 0.0))

{
    if (GLEW_OK != glewInit()) std::cerr << "GLEW INIT Error";
    
    std::vector<std::pair<std::string, GLuint>> paths = {
      std::pair<std::string, GLuint>("src/openGL/shaders/basic.vrtx", GL_VERTEX_SHADER),
      std::pair<std::string, GLuint>("src/openGL/shaders/basic.frgmnt", GL_FRAGMENT_SHADER)
    };
    mainShader = std::make_unique<Shader>(paths);
   
    planeModel = std::make_unique<Model>("assets/models/BGEAR_plane.obj");
    sky = std::make_unique<Sky>("assets/terrain/textures/sky");
    terrain = std::make_unique<Terrain>("assets/terrain/hm.png", camera.get_position());
}

void Frontend3d::update(const Plane& plane) {

}

void Frontend3d::draw(sf::RenderWindow& window) {
    glm::mat4 planeTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 300.0f, 0.0f));

    
    glm::mat4 view = camera.get_view();
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 800.0f/600.0f, 0.1f, 4096.0f);
    glm::vec4 light(-500.0f, 800.0f, 2048.0f, 1.0f);
  
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	sky->render(camera.get_view_no_translate(), projection);
    terrain->draw(camera.get_position(), view, projection, glm::vec3(light));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    planeModel->draw(glm::scale(planeTransform, glm::vec3(10,10,10)), view, projection, 
                camera.get_position(), glm::vec3(view * light));
    glDisable(GL_BLEND);
}
    
