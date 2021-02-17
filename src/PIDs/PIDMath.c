#include "PIDMath.h"
#include "time.h"
#define windup 20
void PIDsetPIDs(float Kp, float Ki, float Kd,float Kl, PIDValues_t *pid){
    pid->Kp =Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->Kl = Kl;
    PIDreset(pid);
}
void PIDreset(PIDValues_t* pid){
    pid->ITerm =0;
    pid->lastError =0;
}
float PIDupdate(float feedback, float setpoint, PIDValues_t* pid, timeUs_t currentTime, timeUs_t lastTime){
 float error = setpoint - feedback;
 timeUs_t deltaTime = currentTime - lastTime;
    pid->ITerm += error*deltaTime;
    if(pid->ITerm>windup)
        pid->ITerm = windup;
    else if(pid->ITerm<-windup)
        pid->ITerm=-windup;
    float Dterm =0;
    if (deltaTime>0)
        Dterm = (error - pid->lastError)/deltaTime;
    pid->lastError = error;
    return pid->Kp*error + pid->Ki*pid->ITerm + pid->Kd*Dterm;
    
}
