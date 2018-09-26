#include "table.h"


Table::Table(std::vector<std::pair<float, float>> keyvals) :
    keyvals{keyvals}
    {

}




float Table::get(float key) {
    if (key < keyvals.front().first)
        return keyvals.front().second;
    if (keyvals.back().first <= key)
        return keyvals.back().second;

    for (int i = 0; i < keyvals.size() - 1; ++i) {
        float key1 = keyvals[i].first;
        float val1 = keyvals[i].second;
        float key2 = keyvals[i+1].first;
        float val2 = keyvals[i+1].second;

        if (key1 <= key && key < key2)
        {
            float ratio = (key - key1) / (key2 - key1);
            float value = val1 + ratio * (val2 - val1);
            return value;
        }
    }

    // this will never be executed
    return 0;
}

