#include "table.h"
#include <cmath>

Table::Entry::Entry(float alpha, float lift, float drag, float axis, float moment) :
                alpha{alpha}, 
                lift{lift}, 
                drag{drag}
{ 
    // copied from coefficients.h in prof. Hans' twodimensional simulator
    if (axis == 0)
        this->moment = moment;
    else
        this->moment = moment - axis * ( lift * cos( alpha ) + drag * sin( alpha ));

}


Table::Entry Table::get(float alpha) {
    if (alpha < entries.front().alpha)
        return entries.front();
    if (entries.back().alpha <= alpha)
        return entries.back();

    for (int i = 0; i < entries.size() - 1; ++i) {
        float alpha1 = entries[i].alpha;
        float alpha2 = entries[i+1].alpha;

        if (alpha1 <= alpha && alpha < alpha2)
        {
            float ratio = (alpha - alpha1) / (alpha2 - alpha1);
            
            return Entry(
                    alpha,
                    entries[i].lift   + ratio * (entries[i+1].lift   - entries[i].lift  ),
                    entries[i].drag   + ratio * (entries[i+1].drag   - entries[i].drag  ),
                    0,
                    entries[i].moment + ratio * (entries[i+1].moment - entries[i].moment)
                     );
        }
    }

    // this will never be executed
    return Entry(0,0,0,0,0);
}

