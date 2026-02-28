#include "Screen.h"
#include "SPI.h"
#include "Font.h"
#include <string.h>
#include <math.h>

volatile LcdOrientation lcd_Orientation = lcdOrientation_Portrait;

void Lcd_WriteData(u8 data)
{
    LCD_DC_H;
    LCD_CS_L;
    Lcd_SpiWrite(data);
    LCD_CS_H;
}
void Lcd_WriteData16(u16 data)
{
    LCD_DC_H;
    LCD_CS_L;
    Lcd_SpiWrite(data >> 8);
    Lcd_SpiWrite(data);
    LCD_CS_H;
}
void Lcd_WriteCom(u8 com)
{
    LCD_DC_L;
    LCD_CS_L;
    Lcd_SpiWrite(com);
    LCD_CS_H;
}

u8 Lcd_Read()
{
    return Lcd_SpiRead();
}

void Lcd_Init()
{
    Spi4_Init();
    // 复位引脚
    rcu_periph_clock_enable(RCU_GPIOF);
    gpio_mode_set(LCD_RST_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_RST_PIN);
    gpio_output_options_set(LCD_RST_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_RST_PIN);
    
    // 数据/命令引脚
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_mode_set(LCD_DC_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_DC_PIN);
    gpio_output_options_set(LCD_DC_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_DC_PIN);
    
    // 片选引脚
    gpio_mode_set(LCD_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_CS_PIN);
    gpio_output_options_set(LCD_CS_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_CS_PIN);
    
    // 背光引脚
    gpio_mode_set(LCD_BLK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_BLK_PIN);
    gpio_output_options_set(LCD_BLK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_BLK_PIN);

    LCD_BLK_H; //关闭背光
    LCD_RST_L; //复位
    delay_1ms(100);
    LCD_RST_H;
    delay_1ms(100); 
 
    Lcd_WriteCom(0x11); // Sleep out
    delay_1ms(120);      // Delay 120ms 

    Lcd_SetOrientation(lcdOrientation_Portrait);

    Lcd_WriteCom(0x3A);
    Lcd_WriteData(0x05);

    Lcd_WriteCom(0xB2);
    Lcd_WriteData(0x0C);
    Lcd_WriteData(0x0C);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x33);
    Lcd_WriteData(0x33);

    Lcd_WriteCom(0xB7);
    Lcd_WriteData(0x35);

    Lcd_WriteCom(0xBB);
    Lcd_WriteData(0x32); // Vcom=1.35V

    Lcd_WriteCom(0xC2);
    Lcd_WriteData(0x01);

    Lcd_WriteCom(0xC3);
    Lcd_WriteData(0x15); // GVDD=4.8V  颜色深度

    Lcd_WriteCom(0xC4);
    Lcd_WriteData(0x20); // VDV, 0x20:0v

    Lcd_WriteCom(0xC6);
    Lcd_WriteData(0x0F); // 0x0F:60Hz

    Lcd_WriteCom(0xD0);
    Lcd_WriteData(0xA4);
    Lcd_WriteData(0xA1);

    Lcd_WriteCom(0xE0);
    Lcd_WriteData(0xD0);
    Lcd_WriteData(0x08);
    Lcd_WriteData(0x0E);
    Lcd_WriteData(0x09);
    Lcd_WriteData(0x09);
    Lcd_WriteData(0x05);
    Lcd_WriteData(0x31);
    Lcd_WriteData(0x33);
    Lcd_WriteData(0x48);
    Lcd_WriteData(0x17);
    Lcd_WriteData(0x14);
    Lcd_WriteData(0x15);
    Lcd_WriteData(0x31);
    Lcd_WriteData(0x34);

    Lcd_WriteCom(0xE1);
    Lcd_WriteData(0xD0);
    Lcd_WriteData(0x08);
    Lcd_WriteData(0x0E);
    Lcd_WriteData(0x09);
    Lcd_WriteData(0x09);
    Lcd_WriteData(0x15);
    Lcd_WriteData(0x31);
    Lcd_WriteData(0x33);
    Lcd_WriteData(0x48);
    Lcd_WriteData(0x17);
    Lcd_WriteData(0x14);
    Lcd_WriteData(0x15);
    Lcd_WriteData(0x31);
    Lcd_WriteData(0x34);
    Lcd_WriteCom(0x21);

    Lcd_WriteCom(0x29);

    delay_1ms(120); // Delay 120ms
}

/**
 * @brief  Lcd_SetOrientation: 设置LCD屏幕的视角方向
 * @param [in] lcdOrientation: 视角方向
 * @return void
 */

void Lcd_SetOrientation(LcdOrientation lcdOrientation)
{
    lcd_Orientation = lcdOrientation;
    Lcd_WriteCom(0x36);
    Lcd_WriteData(lcd_Orientation);
}

/**
 * @brief  Lcd_GetW: 得到当前视角的屏幕宽度
 * @return      u16: 得到屏幕宽度的像素值
 */
u16 Lcd_GetW()
{
    if (lcd_Orientation == lcdOrientation_Portrait || lcd_Orientation == lcdOrientation_APortrait)
    {
        return LCD_W;
    }
    return LCD_H;
}
/**
 * @brief  Lcd_GetW: 得到当前视角的屏幕宽度
 * @return      u16: 得到屏幕高度的像素值
 */
u16 Lcd_GetH()
{
    if (lcd_Orientation == lcdOrientation_Portrait || lcd_Orientation == lcdOrientation_APortrait)
    {
        return LCD_H;
    }
    return LCD_W;
}

/**
 * @brief Lcd_PushStart: 开始写入屏幕数据
 * @param        [in] x: 写入屏幕数据的起始x坐标
 * @param        [in] y: 起始y坐标
 * @param        [in] w: 宽度
 * @param        [in] h: 高度
 * @return void
 */
void Lcd_PushStart(u16 x, u16 y, u16 w, u16 h)
{
    if (lcd_Orientation == lcdOrientation_Portrait || lcd_Orientation == lcdOrientation_APortrait)
    {
        y += 20;
    }
    else if (lcd_Orientation == lcdOrientation_Left || lcd_Orientation == lcdOrientation_Right)
    {
        x += 20;
    }
    w = w - 1;
    h = h - 1;
    Lcd_WriteCom(0x2a); //列地址设置
    Lcd_WriteData16(x);
    Lcd_WriteData16(x + w);
    Lcd_WriteCom(0x2b); //行地址设置
    Lcd_WriteData16(y);
    Lcd_WriteData16(y + h);
    Lcd_WriteCom(0x2c); //储存器写
    LCD_DC_H;
    LCD_CS_L;
}

/**
 * @brief Lcd_Pushu8: 写入屏幕一个字节颜色数据
 * @param [in] color: 颜色数据
 * @return void
 */
void Lcd_Pushu8(u8 color)
{
    Lcd_SpiWrite(color);
}
void Lcd_Pushu16(u16 color)
{
    Lcd_SpiWrite(color >> 8);
    Lcd_SpiWrite(color);
}

/**
 * @brief   Lcd_Pushs: 写入屏幕len个字节颜色数据
 * @param [in] colors: 颜色数据首地址
 * @param [in]  color: 颜色数据个数
 * @return void
 */
void Lcd_Pushs(u16 *colors, u32 len)
{
    while (len != 0)
    {
        Lcd_SpiWrite(*colors >> 8);
        Lcd_SpiWrite(*colors);
        colors++;
        len--;
    }
}
/**
 * @brief   Lcd_PushEnd: 结束写入屏幕数据
 * @return void
 */
void Lcd_PushEnd()
{
    LCD_CS_H;
}