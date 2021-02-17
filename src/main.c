#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "time.h"
#include "test/test.h"
#include "core/tasks.h"
#include "core/Scheduler.h"
#include "setup/setup.h"
#include "JetsonComunication/JetsonComunication.h"


int main(void){
    initSystem();
    testInit();
    taskInit(); //initialize scheduler
    initJetsonCom();
    while (1)
    {
      scheduler();  // call scheduler forever
    }
    
}   