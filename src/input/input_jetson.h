#pragma once
#include <stdint.h>
typedef struct{
    float roll;
    float pitch;
    float yaw;
    float vertical;
    float horizontal;
} setpoints_t;

uint16_t* InputGetPadData(void);

setpoints_t *InputGetSetpoints(void);