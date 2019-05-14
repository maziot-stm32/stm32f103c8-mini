/*
 * bsp_led.c
 *
 *  Created on: 2019年5月14日
 *      Author: Paul
 */

#include "main.h"
#include "bsp_led.h"

/**
 * @brief  初始LED模块
 * @retval void
 */
void led_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  /* GPIO Ports Clock Enable */
  LED_GPIO_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);

  /*Configure GPIO pin : LED_PIN */
  GPIO_InitStruct.Pin = LED_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}

/**
 * @brief  点亮LED
 * @retval void
 */
void led_on(void)
{
  HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET);
}

/**
 * @brief  熄灭LED
 * @retval void
 */
void led_off(void)
{
  HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
}

