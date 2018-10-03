#ifndef PLOT_H
#define PLOT_H
#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
#include <queue>
#include <utility>

class Plot {
public: 
    void draw(sf::RenderWindow& window);
    void add(std::pair<double, double> value);
    Plot(unsigned int width, unsigned int height);
    void setPosition(unsigned int x, unsigned int y);
private:
    unsigned int x;
    unsigned int y;
    unsigned int height;
    unsigned int width; // width of plot in pixels
    double interval; // time interval in seconds
    std::vector< std::pair<double, double> > values; // queue of points(time, y-value)
    sf::VertexArray vertices; //array of vertices that will be drawn
    double maxY; // Used to dynamicly resize the plot window 
};

inline void Plot::setPosition(unsigned int _x, unsigned _y) {x = _x; y = _y;}
#endif
