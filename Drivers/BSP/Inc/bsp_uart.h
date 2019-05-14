/*
 * bsp_usart.h
 *
 *  Created on: 2019年5月14日
 *      Author: Paul
 */

#ifndef BSP_INC_BSP_UART_H_
#define BSP_INC_BSP_UART_H_

#include "main.h"
#include <stdio.h>

void uart_init(void);
void uart_gpio_init(UART_HandleTypeDef* huart);

#endif /* BSP_INC_BSP_UART_H_ */
