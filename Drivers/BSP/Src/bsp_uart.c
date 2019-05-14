/*
 * bsp_usart.c
 *
 *  Created on: 2019年5月14日
 *      Author: Paul
 */

#include <bsp_uart.h>

UART_HandleTypeDef huart1;

/**
 * @brief  初始uart模块
 * @retval void
 */
void uart_init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    uart_gpio_init(&huart1);

    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief  根据uart通路配置对应的gpio
 * @retval void
 */
void uart_gpio_init(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (huart->Instance == USART1)
    {
        /* Peripheral clock enable */
        __HAL_RCC_USART1_CLK_ENABLE()
        ;
        __HAL_RCC_GPIOA_CLK_ENABLE()
        ;

        /**
         * USART1 GPIO Configuration
         * PA9  ------> USART1_TX
         * PA10 ------> USART1_RX
         */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

/**
 * @brief  重定向__io_putchar，重定向后可使用printf函数
 * @retval void
 */
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart1, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
    return (ch);
}
