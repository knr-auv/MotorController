#include "core/error_handler.h"
#include "setup.h"
#include "stm32f4xx_gpio.h"
#include "time.h"
#include "setup.h"
static timeUs_t = last_time;
static void blink(timeUs_t tUs){
    if(tUs>=micros()-last_time){
        GPIO_ToggleBits(LED_BUS,LED_PIN);
        last_time = micros();
    }
}

void ERRORHandle(void){

}
