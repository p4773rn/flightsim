#include "frontend2d.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <glm/gtc/noise.hpp>
#include "misc.h"
#include <sstream>

using std::cout;
using std::endl;

Frontend2d::Frontend2d() {
    if (!planeTexture.loadFromFile("openGL/assets/plane.png"))
        std::cerr << "Error: plane planeTexture not loaded" << std::endl;
    planeSprite.setTexture(planeTexture);
    planeSprite.scale(sf::Vector2f(0.08, 0.08));

    sf::Vector2f size(planeTexture.getSize().x, planeTexture.getSize().y);
    planeSprite.setOrigin(size/2.0f);

    std::string fontFilepath = "openGL/assets/DejaVuSansMono.ttf";
    if(!font.loadFromFile(fontFilepath)) {
        std::cerr << "ERROR: font at " << fontFilepath << " not loaded" << std::endl;
    }
    hudText.setFont(font);
}

void Frontend2d::update(const Plane& plane) {
    // TODO: remove old points
    points.push_back({float(plane.getPos().getX()), -float(plane.getPos().getY())});
    pitch = plane.getAngle();

    altitude = plane.getPos().getY();
    velocity = plane.getVelocity();
    throttle = plane.getThrottle();
    elevatorDeflection = plane.getElevatorDeflection();
    flaps = plane.getFlaps();
    brakes = plane.getBrakesStatus();
}


void Frontend2d::updateCamera(sf::RenderWindow& window) {
    float aspectRatio = window.getDefaultView().getSize().x / 
                        window.getDefaultView().getSize().y;
    
    cameraSize.x = 100 * scale;
    cameraSize.y = cameraSize.x / aspectRatio;
    
    cameraPos = points.back();
    cameraPos.x -= cameraSize.x * 0.3;

    sf::View view = window.getView();
    view.setCenter(cameraPos);
    view.setSize(cameraSize);
    window.setView(view);
}

void Frontend2d::draw(sf::RenderWindow& window) {
    drawSky(window);
    drawGrid(window);
    drawGround(window);
    drawTrail(window);
    drawSprites(window);
    drawHud(window);
}


void Frontend2d::drawTrail(sf::RenderWindow& window) {
    sf::Vertex vertices[points.size()];
    int vertexCount = 0;
   

    for (auto iter = points.rbegin(); iter != points.rend(); ++iter) {
        if (iter != points.rbegin() && std::prev(iter)->x < (cameraPos.x - cameraSize.x/2))
            break;

        int i = points.size() - vertexCount - 1;
        vertices[i].position = *iter;
        vertices[i].color = sf::Color(54, 205, 196, 255);
        vertexCount++;
    }


    window.draw(&vertices[points.size() - vertexCount], vertexCount, sf::LineStrip);
}


void Frontend2d::drawGrid(sf::RenderWindow& window) {
    float cellSize = 100; // grid cell size in meters
    sf::Vector2f gridSize = cameraSize / cellSize + sf::Vector2f(1, 1);

    sf::Vector2f corner = cameraPos - cameraSize / 2.0f;
    corner /= cellSize;
    corner.x = std::floor(corner.x);
    corner.y = std::floor(corner.y);
    corner *= cellSize;
    corner.y += 1; // shift the grid 1 m down

    sf::VertexArray vertices(sf::Lines);
    sf::Color color(54, 205, 196, 255);

    for (int i = 0; i < gridSize.x; ++i) {
        sf::Vector2f v1(cellSize * i, 0);
        sf::Vector2f v2(cellSize * i, cellSize * gridSize.y);

        vertices.append(sf::Vertex(corner + v1, color));
        vertices.append(sf::Vertex(corner + v2, color));
    }
    
    for (int i = 0; i < gridSize.y; ++i) {
        sf::Vector2f v1(0, cellSize * i);
        sf::Vector2f v2(cellSize * gridSize.x, cellSize * i);

        vertices.append(sf::Vertex(corner + v1, color));
        vertices.append(sf::Vertex(corner + v2, color));
    }

    window.draw(vertices);
}


void Frontend2d::drawSprites(sf::RenderWindow& window) {
    planeSprite.setPosition(points.back());
    planeSprite.setRotation(-pitch / M_PI * 180);
    window.draw(planeSprite);


}


void Frontend2d::drawGround(sf::RenderWindow& window) {
    // depth, roughness, and color of ground layers
    std::vector<std::tuple<float, float, sf::Color>> layers = {
        {  0, 0, sf::Color(30, 30, 30)},
        {  2, 0.1, sf::Color(28, 16, 30, 255)},
        { 50, 1, sf::Color(61, 34, 67, 255)},
        { 120, 1, sf::Color(40, 33, 53, 255)},
        { 250, 1, sf::Color(11, 18, 40, 255)}
    };

    // Color of this layer doesn't matter
    layers.push_back({cameraSize.y/2, 0, sf::Color::Red});

    float cellSize = 50; // grid cell size in meters
    sf::Vector2u gridSize(ceil(cameraSize.x / cellSize) + 4, layers.size());

    sf::Vector2f corner(cameraPos.x - cameraSize.x / 2.0f, 0);
    corner.x /= cellSize;
    corner.x = std::floor(corner.x) - 1;
    corner.x *= cellSize;

    sf::Vector2f grid[gridSize.x][gridSize.y];
    sf::Color colorGrid[gridSize.x][gridSize.y]; // last row and column are not needed but oh well

    for (unsigned int x = 0; x < gridSize.x; ++x) {
        for (unsigned int y = 0; y < gridSize.y; ++y) {
            float depth, roughness;
            sf::Color color;
            std::tie(depth, roughness, color) = layers[y];

            grid[x][y].x = corner.x + x * cellSize;
            grid[x][y].y = corner.y + depth;
    
            float noise = glm::simplex(glm::vec2(grid[x][y].x, grid[x][y].y) * 10.0f);
            grid[x][y].x += noise * cellSize / 2.0f * roughness;
            grid[x][y].y += noise * cellSize / 2.0f * roughness;
            
            color.r = clamp<int>(color.r + noise * 10 , 0, 255);
            color.g = clamp<int>(color.g + noise * 10 , 0, 255);
            color.b = clamp<int>(color.b + noise * 10 , 0, 255);
            colorGrid[x][y] = color;
        }
    }


    sf::VertexArray vertices(sf::Quads);
    for (unsigned int x = 0; x < gridSize.x - 1; ++x) {
        for (unsigned int y = 0; y < gridSize.y - 1; ++y) {
            sf::Vector2f v1(grid[x][y]);
            sf::Vector2f v2(grid[x+1][y]);
            sf::Vector2f v3(grid[x+1][y+1]);
            sf::Vector2f v4(grid[x][y+1]);

            sf::Color color = colorGrid[x][y];

            vertices.append(sf::Vertex(v1, color));
            vertices.append(sf::Vertex(v2, color));
            vertices.append(sf::Vertex(v3, color));
            vertices.append(sf::Vertex(v4, color));
        }
    }
    
    window.draw(vertices);
}




void Frontend2d::drawSky(sf::RenderWindow& window) {
    // height and color of sky layers
    std::vector<std::tuple<float, sf::Color>> layers = {
        {  0, sf::Color(190, 0, 0, 255)},
        { 10, sf::Color(227, 117, 10, 255)},
        //{ 30, sf::Color(232, 158, 145, 255)},
        { 40, sf::Color(255, 102, 145, 255)},
        { 150, sf::Color(35, 58, 122, 255)},
        { 400, sf::Color(28, 5, 43, 255)}
    };

    float lastHeight;
    sf::Color lastColor;
    std::tie(lastHeight, lastColor) = layers.back();
    layers.push_back({std::max(lastHeight, -cameraPos.y + cameraSize.y / 2.0f), lastColor});

    sf::Vector2f corner(cameraPos.x - cameraSize.x / 2.0f, 0);
    
    sf::VertexArray vertices(sf::Quads);
    for (unsigned int i = 0; i < layers.size() - 1; ++i) {
        float h1, h2;
        sf::Color c1, c2;
        std::tie(h1, c1) = layers[i];
        std::tie(h2, c2) = layers[i+1];

        sf::Vector2f v1(0, -h1);
        sf::Vector2f v2(cameraSize.x, -h1);
        sf::Vector2f v3(cameraSize.x, -h2);
        sf::Vector2f v4(0, -h2);

        vertices.append(sf::Vertex(corner + v1, c1));
        vertices.append(sf::Vertex(corner + v2, c1));
        vertices.append(sf::Vertex(corner + v3, c2));
        vertices.append(sf::Vertex(corner + v4, c2));
    }

    
    window.draw(vertices);
}


void Frontend2d::drawHud(sf::RenderWindow& window) {
    hudView.setSize(window.getSize().x, window.getSize().y);
    hudView.setCenter(hudView.getSize() * 0.5f);
    window.setView(hudView);
    
    std::ostringstream sout;
    sout.precision(2);
    sout.flags(std::ios_base::fixed);

    sout << "Altitude: " << altitude << endl;
    sout << "Vel: " << velocity << endl;
    sout << "Pitch: " << rad2deg(pitch) << endl;
    sout << endl;
    sout << "Throttle: " << throttle << endl;
    sout << "Elevator: " << elevatorDeflection << endl;
    sout << "Flaps: " << flaps << endl;
    sout << "Brakes: " << brakes << endl;

    hudText.setString(sout.str());
    window.draw(hudText);

    window.setView(window.getDefaultView());
}






