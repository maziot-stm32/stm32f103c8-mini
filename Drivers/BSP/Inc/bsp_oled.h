#ifndef __BSP_OLED_H
#define __BSP_OLED_H

#include "main.h"

#define OLED_SCL_PIN            GPIO_PIN_9
#define OLED_SCL_PORT           GPIOB
#define OLED_SCL_CLK_ENABLE     __HAL_RCC_GPIOB_CLK_ENABLE

#define OLED_SDA_PIN            GPIO_PIN_8
#define OLED_SDA_PORT           GPIOB
#define OLED_SDA_CLK_ENABLE     __HAL_RCC_GPIOB_CLK_ENABLE

#define OLED_RST_PIN            GPIO_PIN_7
#define OLED_RST_PORT           GPIOB
#define OLED_RST_CLK_ENABLE     __HAL_RCC_GPIOB_CLK_ENABLE

#define OLED_DC_PIN             GPIO_PIN_6
#define OLED_DC_PORT            GPIOB
#define OLED_DC_CLK_ENABLE      __HAL_RCC_GPIOB_CLK_ENABLE

void oled_delay();

void oled_write_dat(uint8_t dat);
void oled_write_cmd(uint8_t cmd);
void oled_display_on(void);
void oled_display_off(void);
void oled_refresh_gram(void);
void oled_init(void);
void oled_clear_screen(void);
void oled_draw_point(uint8_t x, uint8_t y, uint8_t t);
void oled_draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
        uint8_t dot);
void oled_draw_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size,
        uint8_t mode);
void oled_draw_number(uint8_t x, uint8_t y, uint32_t num, uint8_t len,
        uint8_t size);
void oled_draw_string(uint8_t x, uint8_t y, char *p);

#endif /* __BSP_OLED_H */
