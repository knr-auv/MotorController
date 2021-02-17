#include <stdbool.h>
void USART2_Transmit_DMA(uint8_t* tx_buffer, uint16_t len);

void USART2_Receive_DMA(uint8_t *rx_buffer, uint16_t len);  //once enabled it will serve forever untill stopreceiving will be called
void USART2_StopReceiving();

bool USART2_Check_Tx_end();                 //its important to check it before new operation

uint16_t USART2_GetReceivedBytes();         //returns number of bytes that sits in rx buffer

uint16_t USART2_GetSkippedFrames();         //return number of unhandled frames. Frames is considered handled if USART2_GetReceivedBytes is called
