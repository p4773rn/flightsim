#ifndef PLOT_H
#define PLOT_H
#include <SFML/Graphics.hpp>

#include <queue>
#include <utility>

class Plot {
public:
    unsigned int x;
    unsigned int y;
    unsigned int height;
    unsigned int width; // width of plot in pixels
    float interval; // time interval in seconds
    std::queue< std::pair<float, float> > values; // queue of points(time, y-value)
    void draw(sf::RenderWindow& window);
    void add(std::pair<float, float> value);
    Plot(unsigned int width, unsigned int height);
    float maxY; // Used to dynamicly resize the plot window 
};


#endif