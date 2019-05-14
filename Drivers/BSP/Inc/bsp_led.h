/*
 * bsp_led.h
 *
 *  Created on: 2019年5月14日
 *      Author: Paul
 */

#ifndef BSP_INC_BSP_LED_H_
#define BSP_INC_BSP_LED_H_

#include "main.h"

#define LED_GPIO_PIN          GPIO_PIN_13
#define LED_GPIO_PORT         GPIOC
#define LED_GPIO_CLK_ENABLE   __HAL_RCC_GPIOC_CLK_ENABLE

void led_init(void);
void led_on(void);
void led_off(void);

#endif /* BSP_INC_BSP_LED_H_ */
