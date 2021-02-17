#include "math/Quaternions.h"
//#include "IMU/bno080.h"
static Quaternion attitude;

Quaternion AtGetCurrentAttitude(void){
return attitude;
}
void AtHandleNewData(float r, float i, float j, float k){
    attitude.r = r;
    attitude.i = i;
    attitude.j = j;
    attitude.k = k;
}