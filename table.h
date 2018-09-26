#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <utility>

class Table {
public:
    // keys must be distinct or division by zero will happen
    Table(std::vector<std::pair<float, float>> keyvals);
    float get(float key);

private:
    std::vector<std::pair<float, float>> keyvals;
};


#endif
