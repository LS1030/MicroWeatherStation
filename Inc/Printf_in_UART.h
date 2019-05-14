#ifndef __PRINTF_IN_UART_H
#define __PRINTF_IN_UART_H

#include "stdio.h"
#include "usart.h"

#define USE_IDE VisualGDB
#define VisualGDB 0
#define IARorKeil 1

#define USE_USART huart1

void Usart_SendString(uint8_t *str);
void Receive_Return(void);

#endif // !__PRINTF_IN_UART_H

