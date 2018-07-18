#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

extern void usart_config(u32 bound);
extern void usart_send_info(int *usart1_send_buff);
extern void usart1_rx_buff_process();



#endif
