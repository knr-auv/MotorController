#include "PIDs/Controls.h"
static setpoints_t padSetpoints = {0,0,0,0,0};
static Quaternion autonomySetpoints = {0,0,0,0};

setpoints_t* getPadStableSetpoints(void){
    return &padSetpoints;
}

Quaternion* getAutonomySetpoints(void){
    return &autonomySetpoints;
}


