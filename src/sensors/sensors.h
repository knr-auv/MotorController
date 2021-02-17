#include "math/Quaternions.h"

Quaternion AtGetCurrentAttitude(void);
void AtHandleNewData(float r, float i, float j, float k);
float DEPTHGetDepth(void);
float* GetGyroValues(void);
void GyroHandleNewData(float roll, float pitch, float yaw);