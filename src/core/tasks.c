#include "Scheduler.h"
#include "test/test.h"
#include "sensors/sensors.h"
#include "PIDs/PIDController.h"
#include "config.h"
#include "JetsonComunication/JetsonComunication.h"

void taskGyro(timeUs_t currentTimeUs){
for(int i =0; i<100000;i++);
}

void taskPID(timeUs_t currentTimeUs){
PIDLoop(currentTimeUs);
}
void taskTelemetry(timeUs_t currentTimeUs){
handleTelemetry(currentTimeUs);
}
void taskTest(timeUs_t currenTimeUs){
for(int i =0; i<100000;i++);
}
task_t tasks[TASK_COUNT] = {
     [TASK_SYSTEM] = {
        .taskName = "SYSTEM",
        .taskFun = taskSystem,
        .desiredPeriod = TASK_PERIOD_HZ(TASK_SYSTEM_PERIOD_HZ),              // run every 100 ms, 10Hz
        .staticPriority = TASK_PRIORITY_HIGH,
},[TASK_GYRO]={
        .taskName = "GYRO",
        .taskFun = taskGyro,
        .desiredPeriod = TASK_PERIOD_HZ(TASK_GYRO_PERIOD_HZ),          
        .staticPriority = TASK_PRIORITY_HIGH,
},
[TASK_TEST]={
        .taskName = "TEST",
        .taskFun = taskTest,
        .desiredPeriod = TASK_PERIOD_HZ(1),          
        .staticPriority = TASK_PRIORITY_HIGH,
},
[TASK_PID]={
        .taskName = "PID",
        .taskFun = taskPID,
        .desiredPeriod = TASK_PERIOD_HZ(TASK_PID_PERIOD_HZ),           
        .staticPriority = TASK_PRIORITY_HIGH,
},
[TASK_TELEMETRY]={
        .taskName = "TELEMETRY",
        .taskFun = taskTelemetry,
        .desiredPeriod = TASK_PERIOD_HZ(10),          
        .staticPriority = TASK_PRIORITY_HIGH,
},
};

void taskInit(void){
        schedulerInit();
        setTaskEnabled(TASK_PID, true);
        setTaskEnabled(TASK_TEST,true);
        setTaskEnabled(TASK_GYRO,true);
        setTaskEnabled(TASK_TELEMETRY,true);
}