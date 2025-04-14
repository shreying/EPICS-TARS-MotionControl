#ifndef USART_H
#define USART_H

#include <stdint.h>  
#include <stdbool.h>

#include <stdio.h>
#include "stm32f10x.h"

void USART1_Init(void);
int fputc(int ch, FILE *f);
void Delay_ms(uint32_t ms);

#endif