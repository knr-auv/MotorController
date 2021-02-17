#include "time.h"

typedef struct{
    float Kp;
    float Ki;
    float Kd;
    float Kl;
    float ITerm;
    float lastError;
}PIDValues_t;

void PIDsetPIDs(float Kp, float Ki, float Kd, float Kl, PIDValues_t *pid);
float PIDupdate(float feedback, float setpoint, PIDValues_t* pid, timeUs_t currentTime, timeUs_t lastTime);
void PIDreset(PIDValues_t* pid);