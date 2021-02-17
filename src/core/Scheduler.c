/*Scheduler - taken from cleanflight. Adjusted to our needs.*/
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "time.h"
#include "Scheduler.h"

#define TASK_MOVING_SUM_COUNT           32
timeUs_t checkFuncMaxExecutionTime;
timeUs_t checkFuncTotalExecutionTime;
timeUs_t checkFuncMovingSumExecutionTime;

static uint32_t totalWaitingTasks;
static uint32_t totalWaitingTasksSamples;
uint16_t averageSystemLoadPercent;

static task_t* taskQueue[TASK_COUNT]; //keep in mind that at the end we will have null pointer 
static int taskQueueSize =0;
static int taskQueuePos =0;

static task_t *currentTask =NULL;

void clearQueue(void){
    memset(taskQueue,0,sizeof(taskQueue));
    taskQueuePos = 0;
    taskQueueSize = 0;
}

bool isInQueue(task_t* task){
    for(int i =0; i<taskQueueSize;i++){
        if(taskQueue[i]==task)
            return true;
    }
    return false;
}
static bool queueAdd(task_t *task)
{
    if ((taskQueueSize >= TASK_COUNT) || isInQueue(task)) {
        return false;
    }
    for (int ii = 0; ii <= taskQueueSize; ++ii) {
        if (taskQueue[ii] == NULL || taskQueue[ii]->staticPriority < task->staticPriority) {
            memmove(&taskQueue[ii+1], &taskQueue[ii], sizeof(task) * (taskQueueSize - ii));
            taskQueue[ii] = task;
            ++taskQueueSize;
            return true;
        }
    }
    return false;
}
static bool queueRemove(task_t *task)
{
    for (int ii = 0; ii < taskQueueSize; ++ii) {
        if (taskQueue[ii] == task) {
            memmove(&taskQueue[ii], &taskQueue[ii+1], sizeof(task) * (taskQueueSize - ii));
            --taskQueueSize;
            return true;
        }
    }
    return false;
}

/*
 * Returns first item queue or NULL if queue empty
 */
static task_t *queueFirst(void)
{
    taskQueuePos = 0;
    return taskQueue[0]; // guaranteed to be NULL if queue is empty
}

/*
 * Returns next item in queue or NULL if at end of queue
 */
static task_t *queueNext(void)
{
    return taskQueue[++taskQueuePos]; // guaranteed to be NULL at end of queue
}


void taskSystem(timeUs_t currentTimeUS) {
    if (totalWaitingTasksSamples > 0) {
        averageSystemLoadPercent = 100 * totalWaitingTasks / totalWaitingTasksSamples;
        totalWaitingTasksSamples = 0;
        totalWaitingTasks = 0;
    }
}

void scheduleTask(taskID_e taskID, timeDelta_t newPeriodUs){
 if (taskID == TASK_SELF) {
        task_t *task = currentTask;
        task->desiredPeriod = MAX(SCHEDULER_DELAY_LIMIT, newPeriodUs);  // Limit delay to 100us (10 kHz) to prevent scheduler clogging
    } else if (taskID < TASK_COUNT) {
        task_t *task = &tasks[taskID];
        task->desiredPeriod = MAX(SCHEDULER_DELAY_LIMIT, newPeriodUs);  // Limit delay to 100us (10 kHz) to prevent scheduler clogging
    }
}
void setTaskEnabled(taskID_e taskID, bool enabled){
    
 if (taskID == TASK_SELF || taskID < TASK_COUNT) {
        task_t *task = taskID == TASK_SELF ? currentTask : &tasks[taskID];
        if (enabled && task->taskFun) {
            queueAdd(task);
        } else {
            queueRemove(task);
        }
    }
}
void schedulerInit(void){
    clearQueue();
    queueAdd(&tasks[TASK_SYSTEM]);
}

void scheduler(void)
{
    // Cache currentTime
    const timeUs_t currentTimeUs = micros();

    // Check for realtime tasks
    timeUs_t timeToNextRealtimeTask = TIMEUS_MAX;
    for (const task_t *task = queueFirst(); task != NULL && task->staticPriority >= TASK_PRIORITY_REALTIME; task = queueNext()) {
        const timeUs_t nextExecuteAt = task->lastExecutedAt + task->desiredPeriod;
        if ((int32_t)(currentTimeUs - nextExecuteAt) >= 0) {
            timeToNextRealtimeTask = 0;
        } else {
            const timeUs_t newTimeInterval = nextExecuteAt - currentTimeUs;
            timeToNextRealtimeTask = MIN(timeToNextRealtimeTask, newTimeInterval);
        }
    }
    const bool outsideRealtimeGuardInterval = (timeToNextRealtimeTask > 0);

    // The task to be invoked
    task_t *selectedTask = NULL;
    uint16_t selectedTaskDynamicPriority = 0;

    // Update task dynamic priorities
    uint16_t waitingTasks = 0;
    for (task_t *task = queueFirst(); task != NULL; task = queueNext()) {
        // Task has checkFunc - event driven
        if (task->checkFunc) {
            const timeUs_t currentTimeBeforeCheckFuncCallUs = micros();

            // Increase priority for event driven tasks
            if (task->dynamicPriority > 0) {
                task->taskAgeCycles = 1 + ((timeDelta_t)(currentTimeUs - task->lastSignaledAt)) / task->desiredPeriod;
                task->dynamicPriority = 1 + task->staticPriority * task->taskAgeCycles;
                waitingTasks++;
            } else if (task->checkFunc(currentTimeBeforeCheckFuncCallUs, currentTimeBeforeCheckFuncCallUs - task->lastExecutedAt)) {
#ifndef SKIP_TASK_STATISTICS
                const timeUs_t checkFuncExecutionTime = micros() - currentTimeBeforeCheckFuncCallUs;
                checkFuncMovingSumExecutionTime -= checkFuncMovingSumExecutionTime / TASK_MOVING_SUM_COUNT;
                checkFuncMovingSumExecutionTime += checkFuncExecutionTime;
                checkFuncTotalExecutionTime += checkFuncExecutionTime;   // time consumed by scheduler + task
                checkFuncMaxExecutionTime = MAX(checkFuncMaxExecutionTime, checkFuncExecutionTime);
#endif
                task->lastSignaledAt = currentTimeBeforeCheckFuncCallUs;
                task->taskAgeCycles = 1;
                task->dynamicPriority = 1 + task->staticPriority;
                waitingTasks++;
            } else {
                task->taskAgeCycles = 0;
            }
        } else {
            // Task is time-driven, dynamicPriority is last execution age (measured in desiredPeriods)
            // Task age is calculated from last execution
            task->taskAgeCycles = ((timeDelta_t)(currentTimeUs - task->lastExecutedAt)) / task->desiredPeriod;
            if (task->taskAgeCycles > 0) {
                task->dynamicPriority = 1 + task->staticPriority * task->taskAgeCycles;
                waitingTasks++;
            }
        }

        if (task->dynamicPriority > selectedTaskDynamicPriority) {
            const bool taskCanBeChosenForScheduling =
                (outsideRealtimeGuardInterval) ||
                (task->taskAgeCycles > 1) ||
                (task->staticPriority == TASK_PRIORITY_REALTIME);
            if (taskCanBeChosenForScheduling) {
                selectedTaskDynamicPriority = task->dynamicPriority;
                selectedTask = task;
            }
        }
    }

    totalWaitingTasksSamples++;
    totalWaitingTasks += waitingTasks;

    currentTask = selectedTask;

    if (selectedTask) {
        // Found a task that should be run
        selectedTask->lastExecutedAt = (timeDelta_t)(currentTimeUs - selectedTask->lastExecutedAt);
        selectedTask->lastExecutedAt = currentTimeUs;
        selectedTask->dynamicPriority = 0;

        // Execute task
        const timeUs_t currentTimeBeforeTaskCall = micros();
        selectedTask->taskFun(currentTimeBeforeTaskCall);

#ifndef SKIP_TASK_STATISTICS
        const timeUs_t taskExecutionTime = micros() - currentTimeBeforeTaskCall;
        selectedTask->movingSumExecutionTime += taskExecutionTime - selectedTask->movingSumExecutionTime / TASK_MOVING_SUM_COUNT;
        selectedTask->totalExecutionTime += taskExecutionTime;   // time consumed by scheduler + task
        selectedTask->maxExecutionTime = MAX(selectedTask->maxExecutionTime, taskExecutionTime);
#endif

    } else {
        // Execute system real-time callbacks and account for them to SYSTEM account
        const timeUs_t currentTimeBeforeTaskCall = micros();
        //taskRunRealtimeCallbacks(currentTimeBeforeTaskCall);

#ifndef SKIP_TASK_STATISTICS
        selectedTask = &tasks[TASK_SYSTEM];
        const timeUs_t taskExecutionTime = micros() - currentTimeBeforeTaskCall;
        selectedTask->movingSumExecutionTime += taskExecutionTime - selectedTask->movingSumExecutionTime / TASK_MOVING_SUM_COUNT;
        selectedTask->totalExecutionTime += taskExecutionTime;   // time consumed by scheduler + task
        selectedTask->maxExecutionTime = MAX(selectedTask->maxExecutionTime, taskExecutionTime);
#endif

    }
}