
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"
#include <stdbool.h>
#include <string.h>
#include "config.h"


static  uint8_t *txBuff;
static  uint16_t txMsgSize;
static bool txCompleted = true;

static uint8_t DMABuff[TELEMETRY_RX_BUFF_SIZE];
static  uint16_t receivedBytes =0;
static  uint16_t rxLen =0;
static  uint8_t *rxBuff;
static uint16_t skippedFrames = 0; 
static bool shouldRxStop = false;

uint16_t USART2_GetSkippedFrames(void){
    return skippedFrames;
}
uint16_t USART2_GetReceivedBytes(){
    uint16_t ret = receivedBytes;
    receivedBytes = 0;
    return ret;
}

bool USART2_Check_Tx_end(){
    return txCompleted;
}

void USART2_Transmit_DMA( uint8_t* tx_buffer, uint16_t len)
{
    //disable DMA
    DMA1_Stream6->CR &= ~(DMA_SxCR_EN);     //disable DMA
    txCompleted = false;
    txBuff = tx_buffer;
    txMsgSize = len;   
    DMA1_Stream6->M0AR = (uint32_t)tx_buffer;  //set tx_buffer
    DMA1_Stream6->NDTR = txMsgSize;               //set msg len

    DMA1_Stream6->CR |= DMA_SxCR_EN;        
}

void USART2_Receive_DMA(uint8_t *rx_buffer, uint16_t len){
    if(len==0) return;
    shouldRxStop = false;
    DMA1_Stream5->CR&= ~(DMA_SxCR_EN);  
    while(DMA1_Stream5->CR&DMA_SxCR_EN);
    rxBuff = rx_buffer;
    rxLen = len;
    DMA1_Stream5->M0AR = (uint32_t)DMABuff;
    DMA1_Stream5->NDTR = TELEMETRY_RX_BUFF_SIZE; 
    DMA1_Stream5->CR |= DMA_SxCR_EN;   
}
void USART2_StopReceiving(){
DMA1_Stream5->CR&= ~(DMA_SxCR_EN);
shouldRxStop = true; 
}
void USART2_IRQHandler(void)
{
  if (USART2->SR & USART_SR_IDLE)
    {
        USART2->DR;                             //If not read usart willl crush                  
        DMA1_Stream5->CR &= ~DMA_SxCR_EN;       /* Disable DMA on stream 5 */
    }  

}

//rx_interrupt

void DMA1_Stream5_IRQHandler(void)
{
    if(DMA1->HISR & DMA_HISR_TCIF5){ //if interupt is TC
        DMA1->HIFCR = DMA_HIFCR_CTCIF5;     //clear tc flag
        if(receivedBytes!=0)
            skippedFrames++;
        receivedBytes = TELEMETRY_RX_BUFF_SIZE - DMA1_Stream5->NDTR;
        memcpy(rxBuff,DMABuff, receivedBytes);
        memset(DMABuff, 0, sizeof(DMABuff));
        DMA1_Stream5->M0AR = (uint32_t)DMABuff;      
        DMA1_Stream5->NDTR = rxLen;
        if(!shouldRxStop)
            DMA1_Stream5->CR |= DMA_SxCR_EN;                              
    }
}



void DMA1_Stream6_IRQHandler(void)
{
    if(DMA1->HISR & DMA_HISR_TCIF6){ //if interupt is TC
       txCompleted = true;
        DMA1->HIFCR = DMA_HIFCR_CTCIF6;     //clear tc flag
    }
}


