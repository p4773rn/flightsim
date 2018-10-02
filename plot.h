#ifndef PLOT_H
#define PLOT_H
#include <SFML/Graphics.hpp>

#include <queue>
#include <utility>

class Plot {
public: 
    void draw(sf::RenderWindow& window);
    void add(std::pair<double, double> value);
    Plot(unsigned int width, unsigned int height);
    
private:
    unsigned int x;
    unsigned int y;
    unsigned int height;
    unsigned int width; // width of plot in pixels
    double interval; // time interval in seconds
    std::queue< std::pair<double, double> > values; // queue of points(time, y-value)
    double maxY; // Used to dynamicly resize the plot window 
};


#endif
