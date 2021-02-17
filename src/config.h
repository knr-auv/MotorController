#ifndef CONFIG_H_
#define CONFIG_H_
/*Every pin should be defined here*/
#include "stm32f4xx_gpio.h"
#include "stm32f4xx.h"

//pin A5
#define LED_PIN GPIO_Pin_5
#define LED_BUS GPIOA

//telemetry on UART is using A2, A3 pins
//dma is uisng channel 4 stream 6 for tx and channel 4 str. 5 for rx
#define TELEMETRY_UARTx USART2
#define TELEMETRY_UART_PORTS GPIO_Pin_2 | GPIO_Pin_3
#define TELEMETRY_UART_BUS GPIOA
#define TELEMETRY_UART_PIN_SRC1 GPIO_PinSource2
#define TELEMETRY_UART_PIN_SRC2 GPIO_PinSource3
#define TELEMETRY_GPIO_AF GPIO_AF_USART2
#define TELEMETRY_UART_NVIC USART2_IRQn
#define TELEMETRY_RX_BUFF_SIZE 100
//at baud 115200bps  100 bytes is about 10 ms
//target telemetry interval is 20ms, so for now faster protocol is unnecessary

/*Max freq = 10kHz.*/
#define TASK_SYSTEM_PERIOD_HZ 10
#define TASK_GYRO_PERIOD_HZ 10
#define TASK_PID_PERIOD_HZ 1000

#endif