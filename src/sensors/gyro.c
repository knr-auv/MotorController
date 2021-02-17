
#include "math/Quaternions.h"

static float gyro[3];

float* GetGyroValues(void){
    return gyro;
}

void GyroHandleNewData(float roll, float pitch, float yaw){
    gyro[0]=roll;
    gyro[1]=pitch;
    gyro[2]=yaw;
}
