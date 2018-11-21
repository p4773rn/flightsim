#include "frontend2d.h"
#include <iostream>
#include <cmath>

using std::cout;
using std::endl;

void Frontend2d::update(const Plane& plane) {
    positions.push_back({float(plane.getPos().getX()), float(plane.getPos().getY())});
}


void Frontend2d::updateCamera(sf::RenderWindow& window) {
    float aspectRatio = window.getDefaultView().getSize().x / 
                        window.getDefaultView().getSize().y;
    
    cameraSize.x = 100 * scale;
    cameraSize.y = cameraSize.x / aspectRatio;
    
    cameraPos = positions.back();
    cameraPos.x -= cameraSize.x * 0.3;

    sf::View view = window.getView();
    view.setCenter(cameraPos);
    view.setSize(cameraSize.x, -cameraSize.y);
    window.setView(view);
}

void Frontend2d::draw(sf::RenderWindow& window) {
    drawGrid(window);
    drawTrail(window);
}


void Frontend2d::drawTrail(sf::RenderWindow& window) {
    sf::Vertex vertices[positions.size()];
    int vertexCount = 0;
   

    for (auto iter = positions.rbegin(); iter != positions.rend(); ++iter) {
        if (iter != positions.rbegin() && std::prev(iter)->x < (cameraPos.x - cameraSize.x/2))
            break;

        int i = positions.size() - vertexCount - 1;
        vertices[i].position = *iter;
        vertices[i].color = sf::Color::Red;
        vertexCount++;
    }


    window.draw(&vertices[positions.size() - vertexCount], vertexCount, sf::LineStrip);
}


void Frontend2d::drawGrid(sf::RenderWindow& window) {
    float cellSize = 100; // grid cell size in meters
    sf::Vector2f gridSize = cameraSize / cellSize + sf::Vector2f(1, 1);

    sf::Vector2f corner = cameraPos - cameraSize / 2.0f;
    corner /= cellSize;
    corner.x = std::floor(corner.x);
    corner.y = std::floor(corner.y);
    corner *= cellSize;

    sf::VertexArray vertices(sf::Lines);
    sf::Color color(255, 255, 255, 50);

    for (int i = 0; i < gridSize.x; ++i) {
        sf::Vector2f v1(cellSize * i, 0);
        sf::Vector2f v2(cellSize * i, cellSize * gridSize.y);

        vertices.append(sf::Vertex(corner + v1, color));
        vertices.append(sf::Vertex(corner + v2, color));
    }
    
    for (int i = 0; i < gridSize.y; ++i) {
        sf::Vector2f v1(0, cellSize * i);
        sf::Vector2f v2(cellSize * gridSize.y, cellSize * i);

        vertices.append(sf::Vertex(corner + v1, color));
        vertices.append(sf::Vertex(corner + v2, color));
    }

    window.draw(vertices);
}











