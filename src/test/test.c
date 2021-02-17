#include "time.h"
#include "sensors/sensors.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

void test(timeUs_t currentTimeUs){

}


int16_t received_char_counter = 0;
int16_t sent_char_counter = 0;
uint8_t str[500];
static int count =0;




void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_5))
            GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        else
            GPIO_SetBits(GPIOA, GPIO_Pin_5);
        AtHandleNewData(1,0,0,0);
        GyroHandleNewData(2,2,2);
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        
    }
}




void testInit(){
    TIM_TimeBaseInitTypeDef tim;
    NVIC_InitTypeDef nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    TIM_TimeBaseStructInit(&tim);
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_Prescaler = (uint16_t)(100000 - 1);
    tim.TIM_Period = (uint16_t)(1000 - 1);
    TIM_TimeBaseInit(TIM2, &tim);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

    nvic.NVIC_IRQChannel = TIM2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}