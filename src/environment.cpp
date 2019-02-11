#include "environment.h"

double getAirDensity(double height){
    double seaLevelDensity = 1.225;
    double tenThousandMetersDensity = 0.4135;
    double twentyThousandMetersDensity = 0.055;

    double ratio;
    double deltaDensity;
    if (height <= 10000) {
        ratio = height / 10000;
        deltaDensity = seaLevelDensity - tenThousandMetersDensity;
    } else {
        ratio = height / 20000;
        deltaDensity = seaLevelDensity - twentyThousandMetersDensity;
    }

    return seaLevelDensity - (ratio * deltaDensity);
}

double getGroundEffect(double height) {
    // Ground effect's max height is 1/2 of wingspan
    if (height > 15) {
        return 1;
    }
    return 1.0 + ((15 - height) * 0.02);
}