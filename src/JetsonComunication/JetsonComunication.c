#include "time.h"
#include "stm32f4xx_usart.h"
#include "JetsonComunication/uart_tx_rx.h"
#include "config.h"

static  uint8_t rx_buffer[TELEMETRY_RX_BUFF_SIZE];
static  uint8_t tx_buffer[] ={"Hello from usart dma\n"};
static timeUs_t lastTime = 0;

void handleTelemetry(timeUs_t currentTime){
    //f((currentTime - lastTime)>1000000){
        USART2_Transmit_DMA(rx_buffer, USART2_GetReceivedBytes());
        lastTime = currentTime;
  //  }
}


void initJetsonCom(void){
    USART2_Transmit_DMA(tx_buffer,22);
    USART2_Receive_DMA(rx_buffer, TELEMETRY_RX_BUFF_SIZE);
}