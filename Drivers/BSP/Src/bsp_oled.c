#include "bsp_oled.h"
#include "bsp_uart.h"
#include "stdlib.h"
#include "bsp_oled_font.h"
#include "bsp_oled_ocr_font.h"

/**
 * @brief 显存缓冲区
 */
uint8_t OLED_GRAM[128][8];

/**
 * @brief 初始化 OLED
 * @return null
 */
void oled_init(void)
{
    /* 配置 OLED 使用到的引脚为推挽输出模式 */
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    OLED_SCL_CLK_ENABLE()
    ;
    OLED_SDA_CLK_ENABLE()
    ;
    OLED_RST_CLK_ENABLE()
    ;
    OLED_DC_CLK_ENABLE()
    ;

    GPIO_InitStruct.Pin = OLED_SCL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OLED_SCL_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = OLED_SDA_PIN;
    HAL_GPIO_Init(OLED_SDA_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = OLED_RST_PIN;
    HAL_GPIO_Init(OLED_RST_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = OLED_DC_PIN;
    HAL_GPIO_Init(OLED_DC_PORT, &GPIO_InitStruct);

    /* 复位 OLED */
    HAL_GPIO_WritePin(OLED_RST_PORT, OLED_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(OLED_RST_PORT, OLED_RST_PIN, GPIO_PIN_SET);

    /* 复位之后要进行一段时间的延时, 这个延时很重要 */
    HAL_Delay(10);

    /* OLED 初始化代码 */
    oled_write_cmd(0xAE);    // 关闭显示
    oled_write_cmd(0xD5);    // 设置时钟分频因子,震荡频率
    oled_write_cmd(0x80);    // [3:0],分频因子;[7:4],震荡频率
    oled_write_cmd(0xA8);    // 设置驱动路数
    oled_write_cmd(0x3F);    // 默认0X3F(1/64)
    oled_write_cmd(0xD3);    // 设置显示偏移
    oled_write_cmd(0X00);    // 默认为0
    oled_write_cmd(0x40);    // 设置显示开始行 [5:0],行数
    oled_write_cmd(0x8D);    // 电荷泵设置
    oled_write_cmd(0x14);    // bit2，开启/关闭
    oled_write_cmd(0x20);    // 设置内存地址模式
    oled_write_cmd(0x02);    // [1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
    oled_write_cmd(0xA1);    // 段重定义设置,bit0:0,0->0;1,0->127;
    oled_write_cmd(0xC0);    // 设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
    oled_write_cmd(0xDA);    // 设置COM硬件引脚配置
    oled_write_cmd(0x12);    // [5:4]配置
    oled_write_cmd(0x81);    // 对比度设置
    oled_write_cmd(0xEF);    // 1~255;默认0X7F (亮度设置,越大越亮)
    oled_write_cmd(0xD9);    // 设置预充电周期
    oled_write_cmd(0xf1);    // [3:0],PHASE 1;[7:4],PHASE 2;
    oled_write_cmd(0xDB);    // 设置VCOMH 电压倍率
    oled_write_cmd(0x30);    // [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    oled_write_cmd(0xA4);    // 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
    oled_write_cmd(0xA6);    // 设置显示方式;bit0:1,反相显示;0,正常显示
    oled_write_cmd(0xAF);    // 开启显示

    oled_clear_screen();
}

/**
 * @brief OLED 更新显示, 将 buffer 中的数据写入 OLED
 * @return null
 */
void oled_refresh_gram(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        oled_write_cmd(0xb0 + i);
        oled_write_cmd(0x00);
        oled_write_cmd(0x10);
        for (n = 0; n < 128; n++)
            oled_write_dat(OLED_GRAM[n][i]);
    }
}

/**
 * @brief 简单延时函数
 */
void oled_delay()
{
    int t;
    for (t = 100; t > 0; t--)
    {

    }
}

/**
 * @brief 传输数据
 * @param data 发送的数据
 */
void oled_send_byte(uint8_t data)
{
    uint8_t t;
    HAL_GPIO_WritePin(OLED_SCL_PORT, OLED_SCL_PIN, GPIO_PIN_RESET);

    /* 依次发送8个数据值 */
    for (t = 0; t < 8; t++)
    {
        /* 写入数据的最高位 */
        if ((data & 0x80) >> 7)
        {
            HAL_GPIO_WritePin(OLED_SDA_PORT, OLED_SDA_PIN, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(OLED_SDA_PORT, OLED_SDA_PIN, GPIO_PIN_RESET);
        }

        /* 发送完了最高位, 数据左移一个, 次高位变成了新的最高位 */
        data <<= 1;
        oled_delay();

        HAL_GPIO_WritePin(OLED_SCL_PORT, OLED_SCL_PIN, GPIO_PIN_SET);
        oled_delay();
        HAL_GPIO_WritePin(OLED_SCL_PORT, OLED_SCL_PIN, GPIO_PIN_RESET);
        oled_delay();
    }
}

/**
 * @brief 向 OLED 写一个命令
 * @param cmd 命令值
 * @return null
 */
void oled_write_cmd(uint8_t cmd)
{
    HAL_GPIO_WritePin(OLED_DC_PORT, OLED_DC_PIN, GPIO_PIN_RESET);
    oled_send_byte(cmd);
    HAL_GPIO_WritePin(OLED_DC_PORT, OLED_DC_PIN, GPIO_PIN_SET);
}

/**
 * @brief 向 OLED 写一个数据
 * @param dat 数据值
 * @return null
 */
void oled_write_dat(uint8_t dat)
{
    HAL_GPIO_WritePin(OLED_DC_PORT, OLED_DC_PIN, GPIO_PIN_SET);
    oled_send_byte(dat);
    HAL_GPIO_WritePin(OLED_DC_PORT, OLED_DC_PIN, GPIO_PIN_SET);
}

/**
 * @brief OLED 开显示
 * @return null
 */
void oled_display_on(void)
{
    oled_write_cmd(0x8D);
    oled_write_cmd(0x14);
    oled_write_cmd(0xAF);
}

/**
 * @brief OLED 关显示
 * @return null
 */
void oled_display_off(void)
{
    oled_write_cmd(0x8D);
    oled_write_cmd(0x10);
    oled_write_cmd(0xAE);
}

/**
 * @brief 清除 OLED 的显示
 * @return null
 */
void oled_clear_screen(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
        for (n = 0; n < 128; n++)
            OLED_GRAM[n][i] = 0x00;
    oled_refresh_gram();
}

/**
 * @brief 绘制一个点
 * @param x, y 点坐标
 * @param t 显示模式(写 0 还是写 1 )
 * @return null
 */
void oled_draw_point(uint8_t x, uint8_t y, uint8_t t)
{
    uint8_t pos, bx, temp = 0;
    if (x > 127 || y > 63)
    {
        return;
    }

    pos = 7 - y / 8;
    bx = y % 8;
    temp = 1 << (7 - bx);
    if (t)
        OLED_GRAM[x][pos] |= temp;
    else
        OLED_GRAM[x][pos] &= ~temp;
}

/**
 * @brief 绘制一个矩形
 * @param x1, y1 矩形左上角坐标
 * @param x2, y2 矩形右下角坐标
 * @param dot 显示模式(写 0 还是写 1 )
 * @return null
 */
void oled_draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
        uint8_t dot)
{
    uint8_t x, y;
    for (x = x1; x <= x2; x++)
    {
        for (y = y1; y <= y2; y++)
            oled_draw_point(x, y, dot);
    }
    oled_refresh_gram();
}

/**
 * @brief 绘制一个字符
 * @param x, y 字符(左上角)起始坐标
 * @param chr 要绘制的字符
 * @param size 字体大小选择(可选 12 或 16)
 * @param mode 显示模式(写 0 还是写 1 )
 * @return null
 */
void oled_draw_char(uint8_t x, uint8_t y, uint8_t ch, uint8_t height,
        uint8_t mode)
{
    uint8_t temp = 0;
    uint8_t i;
    uint8_t j;
    uint8_t width = height / 2;
    uint8_t length = ((height + 7) / 8) * width;
    uint8_t y_start = y;
    uint8_t local_mode = mode;

    if (32 != height)
    {
        ch = ch - ' ';
    }

    for (i = 0; i < length; i++)
    {
        if (12 == height)
        {
            temp = oled_asc2_1206[ch][i];
        }
        else if (16 == height)
        {
            temp = oled_asc2_1608[ch][i];
        }
        else if (32 == height)
        {
            temp = font_ocr_32x16[ch][i];
        }

        for (j = 0; j < 8; j++)
        {
            local_mode = (temp & 0x80) ? mode : !mode;
            oled_draw_point(x, y, local_mode);
            temp = temp << 1;
            y++;
            if ((y - y_start) == height)
            {
                y = y_start;
                x++;
                break;
            }
        }
    }
}

/**
 * @brief m^n 函数实现
 * @param m 底数
 * @param n 指数
 */
uint32_t oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--)
        result *= m;
    return result;
}

/**
 * @brief 绘制一个数字
 * @param x, y 数字(左上角)起始坐标
 * @param num 要绘制的数字
 * @param len 要绘制的数字的长度
 * @param size 字体大小选择(可选 12 或 16)
 * @return null
 */
void oled_draw_number(uint8_t x, uint8_t y, uint32_t num, uint8_t len,
        uint8_t size)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if ((enshow == 0) & (t < (len - 1)))
        {
            if (temp == 0)
            {
                oled_draw_char(x + (size / 2) * t, y, ' ', size, 1);
                continue;
            }
            else
                enshow = 1;
        }
        oled_draw_char(x + (size / 2) * t, y, temp + '0', size, 1);
    }
}

/**
 * @brief 显示字符串, 具备自动换行功能
 * @param x, y 字符串(左上角)起始坐标
 * @param p 字符串指针
 * @return null
 */
void oled_draw_string(uint8_t x, uint8_t y, char *p)
{
    while (*p != '\0')
    {
        if (x > 122)
        {
            x = 0;
            y += 16;
        }
        if (y > 58)
        {
            y = 0;
            x = 0;
            oled_clear_screen();
        }
        oled_draw_char(x, y, *p, 12, 1);
        x += 8;
        p++;
    }
}
