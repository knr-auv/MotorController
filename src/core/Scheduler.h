#pragma once
#include <stdbool.h>

#include "time.h"
#ifdef __cplusplus
extern "C" {
#endif
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define SCHEDULER_DELAY_LIMIT 10
#define TIMEUS_MAX UINT64_MAX
#define TASK_PERIOD_HZ(hz) (1000000 / (hz))
#define TASK_PERIOD_MS(ms) ((ms) * 1000)
#define TASK_PERIOD_US(us) (us)

typedef enum {
	TASK_PID,		//task for calculating PID loop
	TASK_GYRO,		//integrating attitude
	TASK_ACC,		//correcting attitude
	TASK_MAG,		//correcting attitude
	TASK_DEPTH,		//correcting depth
	TASK_TELEMETRY,	//sending telemetry
	TASK_SERIAL,
	TASK_SYSTEM,	//task for calculating system load
	TASK_TEST,		
	TASK_COUNT,
	TASK_SELF 		//editing current task
}taskID_e;

typedef enum {
	TASK_PRIORITY_IDLE = 0,     // Disables dynamic scheduling, task is executed only if no other task is active this cycle
	TASK_PRIORITY_LOW = 1,
	TASK_PRIORITY_MEDIUM = 3,
	TASK_PRIORITY_MEDIUM_HIGH = 4,
	TASK_PRIORITY_HIGH = 5,
	TASK_PRIORITY_REALTIME = 6,
	TASK_PRIORITY_MAX = 255
}taskPriority_e;

typedef struct {
	const char* taskName;
	bool isActive;
	void (*taskFun)(timeUs_t currentTime);
	bool (*checkFunc)(timeUs_t currentTimeUs, timeDelta_t currentDeltaTimeUs); //if time is eventy driven this function checks if it should be scheduled
	timeUs_t lastExecutedAt;	//last execution time
	timeUs_t lastSignaledAt;	//if task is event driven this is time when event has trigered
	timeDelta_t desiredPeriod;	//desired period of execution
	uint16_t taskAgeCycles;
	uint8_t staticPriority;		//taskPriority_e
	uint16_t dynamicPriority; 	//Resolve issue of task being not executed due to other task with same priority but lower index
	//stats
	timeUs_t maxExecutionTime;
	timeUs_t totalExecutionTime;
	timeUs_t movingSumExecutionTime; //sum of time consumed by 32 invoccations
}task_t;

extern task_t tasks[TASK_COUNT];
void schedulerInit(void);									//init scheduler
void scheduleTask(taskID_e taskID, timeDelta_t periodUs);	//set task period
void setTaskEnabled(taskID_e taskID, bool enabled);			//enable or disable tasks
void scheduler(void);										//do tasks

void taskSystem(timeUs_t currentTimeUs);

#ifdef __cplusplus
}
#endif