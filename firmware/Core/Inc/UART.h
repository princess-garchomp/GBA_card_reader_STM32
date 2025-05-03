#ifndef UART_H
#define UART_H

#include "stm32f1xx.h"
#include "configuration.h"

void generic_USART_config(USART_TypeDef * USARTx, uint32_t BAUD_SET);

void config_UART1_blocking_9600baud();
void config_UART1_blocking_115200baud();
void UART_BYTE_send_blocking(USART_TypeDef *USARTx, uint8_t input_byte);
uint8_t UART_BYTE_recieve_blocking(USART_TypeDef *USARTx);

void config_UART1_interrupt(uint32_t BAUD_SET);
void UART_send_byte_interrupt_start(USART_TypeDef *USARTx, uint8_t input_byte);
void UART_interrupt_byte_send(USART_TypeDef *USARTx);
uint8_t UART_interrupt_byte_recieve(USART_TypeDef *USARTx);

uint8_t get_last_USART_RX();
void set_USART_TX(uint8_t input);

void UART_DMA_TX_RX_INTERRUPT();
void UART_DMA_START();
void UART_DMA_RESET();


#endif
