#include "input/input_jetson.h"

static uint16_t padData[5];
static setpoints_t setpoints = {0,0,0,0,0};

uint16_t* InputGetPadData(void){
return padData;
}

setpoints_t *InputGetSetpoints(void){
return &setpoints;
}