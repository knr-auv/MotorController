#include "time.h"
typedef enum{
    MODE_STABLE,
    MODE_ACRO,
    MODE_AUTONOMY,
    MODE_COUNT
}modes_e;
typedef enum{
    PID_ROLL,
    PID_PITCH,
    PID_YAW,
    PID_DEPTH,
    PID_COUNT
}pid_t;

void changeMode(modes_e mode);
void PIDLoop(timeUs_t currentTimeUS);
void PIDLoopInit(timeUs_t currentTimeUS);
void PIDLoopSetup(void);
void PIDLoopUpdatePIDs(float Kp, float Ki, float Kd, float Kl, pid_t Controller);