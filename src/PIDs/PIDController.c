#include "math/Quaternions.h"
#include "PIDController.h"
#include "time.h"
#include "Controls.h"
#include "PIDMath.h"
#include "sensors/sensors.h"
#include <stddef.h>

static void MODE_Stable(timeUs_t currentTimeUS);
static void MODE_Acro(timeUs_t currentTimeUS);
static void MODE_Autonomy(timeUs_t currentTimeUS);

static timeUs_t lastTime = 0;
static modes_e current_mode = MODE_STABLE;

static 	void (*ModeHandler[MODE_COUNT])(timeUs_t currentTimeUs) = {MODE_Stable,MODE_Acro,MODE_Autonomy};

static float ref_roll_vel =0;
static float ref_pitch_vel = 0;
static float ref_yaw_vel =0;
static float thrust_out = 0;
static float vertical_out = 0;
static float roll_diff,pitch_diff,yaw_diff;
//static uint16_t motors_out[5] = {0,0,0,0,0};
static PIDValues_t PIDGains[PID_COUNT];
static float ref_heading = 0;
static float ref_depth = 0;

void updateMotors(void){

}

void changeMode(modes_e mode){
    current_mode = mode;
}
void PIDLoopUpdatePIDs(float Kp, float Ki, float Kd, float Kl, pid_t Controller){
    PIDsetPIDs(Kp,Ki,Kd,Kl,&PIDGains[Controller]);
}
void PIDLoop(timeUs_t currentTimeUS) {
    ModeHandler[current_mode](currentTimeUS);
    float *gyro = GetGyroValues();
    roll_diff = PIDupdate(gyro[0],ref_roll_vel, &PIDGains[PID_ROLL],currentTimeUS, lastTime);
    pitch_diff = PIDupdate(gyro[2],ref_pitch_vel, &PIDGains[PID_PITCH],currentTimeUS, lastTime);
    yaw_diff = PIDupdate(gyro[2],ref_yaw_vel, &PIDGains[PID_YAW],currentTimeUS, lastTime);
    updateMotors();
    lastTime = currentTimeUS;
}
void PIDLoopInit(timeUs_t currentTimeUS) {

}
void PIDLoopSetup(void){

}


static void MODE_Stable(timeUs_t currentTimeUS){
    setpoints_t *pad_input = getPadStableSetpoints();
    float currentAtEuler[3];
    Quaternion currentAt = AtGetCurrentAttitude();
    Quaternion refAt = QFromEuler(pad_input->roll, pad_input->pitch, ref_heading);
    float currentDepth = DEPTHGetDepth();

    QToEuler(&refAt,currentAtEuler);
    QConjugate(&currentAt);
    QMultiply(&currentAt,&refAt,&refAt); //refAt is error attitude now
    ref_roll_vel = refAt.i*PIDGains[PID_ROLL].Kl;
    ref_pitch_vel = refAt.j*PIDGains[PID_PITCH].Kl;

    if(pad_input->yaw==0)
        ref_yaw_vel = refAt.i*PIDGains[PID_YAW].Kl;
    else{
        ref_yaw_vel = pad_input->yaw;
        ref_heading = currentAtEuler[2];
    }
    if(pad_input->vertical ==0)
        vertical_out = -PIDupdate(currentDepth,ref_depth,&PIDGains[PID_DEPTH],currentTimeUS, lastTime);
    else{
        vertical_out = pad_input->vertical;
        ref_depth = currentDepth;
    }
    thrust_out = pad_input->horizontal;
    
}

static void MODE_Acro(timeUs_t currentTimeUS){

}

static void MODE_Autonomy(timeUs_t currentTimeUS){

}

