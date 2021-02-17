#include "setup.h"
#include "misc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx.h"
#include "time.h"
#include "config.h"


static void initRCC(){
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);    //DMA 1
}

static void initLEDPin(){
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin = LED_PIN;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(LED_BUS,&gpio);

}

void initTelemetryUart(){
    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Pin = TELEMETRY_UART_PORTS;
    gpio.GPIO_OType = GPIO_OType_PP; //push pull
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    gpio.GPIO_Speed	= GPIO_Speed_50MHz;
    GPIO_Init(TELEMETRY_UART_BUS,&gpio);
    GPIO_PinAFConfig(TELEMETRY_UART_BUS, TELEMETRY_UART_PIN_SRC1, TELEMETRY_GPIO_AF);
    GPIO_PinAFConfig(TELEMETRY_UART_BUS, TELEMETRY_UART_PIN_SRC2, TELEMETRY_GPIO_AF);

        /* UART2  used for telemetry*/
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate 	              = 115200;
	USART_InitStructure.USART_HardwareFlowControl     = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                    = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity                  = USART_Parity_No; 
	USART_InitStructure.USART_StopBits                = USART_StopBits_1;
	USART_InitStructure.USART_WordLength              = USART_WordLength_8b;  
    
    USART_ITConfig(TELEMETRY_UARTx, USART_IT_IDLE, ENABLE);

	USART_Init(TELEMETRY_UARTx, &USART_InitStructure);  
    
 

    NVIC_InitTypeDef NVIC_InitStruct; //NVIC structure variable
  
  	/* Enable the USARTx Interrupt */
  	NVIC_InitStruct.NVIC_IRQChannel = TELEMETRY_UART_NVIC; 
  	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0; 
 	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
 	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 
 	NVIC_Init(&NVIC_InitStruct); 
    
    USART_Cmd(TELEMETRY_UARTx, ENABLE); 

    //enable DMA channel4 stream 6 for tx -> datasheet dma assignment
    //std periph sucks for DMA

    DMA1_Stream6->CR = DMA_SxCR_CHSEL_2 | DMA_SxCR_MINC | DMA_SxCR_DIR_0;   //channel4, memory incrementation, mem ->periph
    DMA1_Stream6->PAR = (uint32_t)&(USART2->DR);                            //source is always usart2 data register
    DMA1_Stream6->CR |= DMA_IT_TC;//|DMA_IT_HT;                              //enable half transfer and transfer complete interrupts
   
    //enable DMA channel4 stream 5 for 7tx
    DMA1_Stream5->CR = DMA_SxCR_CHSEL_2 | DMA_SxCR_MINC;      //channel4, memory incrementation, circular buffer
    DMA1_Stream5->PAR = (uint32_t)&(USART2->DR); 
    DMA1_Stream5->CR |= DMA_IT_TC;                                //half transfer and transfer complete interrupt      

    USART2->CR3 = USART_CR3_DMAT|USART_CR3_DMAR;                           //enable usart dma TX and RX
    
    
    NVIC_InitStruct.NVIC_IRQChannel = DMA1_Stream6_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);

     NVIC_InitStruct.NVIC_IRQChannel = DMA1_Stream5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);

}




void initSystem(){     /*This function is initializing all peripherals */
    SystemInit();
    TicksInit();
    SCB->CPACR |= ((3 << 10*2)|(3 << 11*2)); //init FPU
    initRCC(); 
    initTelemetryUart();

    initLEDPin(); 
}