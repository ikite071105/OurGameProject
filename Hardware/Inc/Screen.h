#ifndef SCREEN_H
#define SCREEN_H

#include "main.h"

#define LCD_W 240
#define LCD_H 280
 
// 数据/命令控制引脚
#define LCD_DC_PORT         GPIOA
#define LCD_DC_PIN          GPIO_PIN_6

#define LCD_DC PA6
#define LCD_DC_L gpio_bit_write(LCD_DC_PORT, LCD_DC_PIN, RESET)
#define LCD_DC_H gpio_bit_write(LCD_DC_PORT, LCD_DC_PIN, SET)

// 片选引脚
#define LCD_CS_PORT         GPIOA
#define LCD_CS_PIN          GPIO_PIN_4

#define LCD_CS PA4
#define LCD_CS_L gpio_bit_write(LCD_CS_PORT, LCD_CS_PIN, RESET)
#define LCD_CS_H gpio_bit_write(LCD_CS_PORT, LCD_CS_PIN, SET)

// 复位引脚
#define LCD_RST_PORT        GPIOF
#define LCD_RST_PIN         GPIO_PIN_10

#define LCD_RST PF10
#define LCD_RST_L gpio_bit_write(LCD_RST_PORT, LCD_RST_PIN, RESET)
#define LCD_RST_H gpio_bit_write(LCD_RST_PORT, LCD_RST_PIN, SET)

// 背光控制引脚
#define LCD_BLK_PORT        GPIOA
#define LCD_BLK_PIN         GPIO_PIN_7

#define LCD_BLK PA7
#define LCD_BLK_L gpio_bit_write(LCD_BLK_PORT, LCD_BLK_PIN, RESET)
#define LCD_BLK_H gpio_bit_write(LCD_BLK_PORT, LCD_BLK_PIN, SET)

#define Lcd_SpiRead Spi4_Read
#define Lcd_SpiWrite Spi4_Write


typedef enum
{
    lcdOrientation_Portrait = 0x00,  //竖屏
    lcdOrientation_APortrait = 0xC0, //竖屏，手机倒过来
    lcdOrientation_Left = 0x70,      //横屏，屏幕在home键左边（比较常用）
    lcdOrientation_Right = 0xA0      //横屏，屏幕在home键右边
} LcdOrientation;

extern volatile LcdOrientation lcd_Orientation;

void Lcd_Init();
void Lcd_SetOrientation(LcdOrientation lcdOrientation);
u16 Lcd_GetW();
u16 Lcd_GetH();

void Lcd_WriteData(u8 data);
void Lcd_WriteData16(u16 data);
void Lcd_WriteCom(u8 com);
u8 Lcd_Read();

//开始写入屏幕数据的起始地址，设置屏幕的显示区域。
void Lcd_PushStart(u16 x, u16 y, u16 w, u16 h);
//写入单个字节的颜色数据到屏幕。
void Lcd_Pushu8(u8 color);
//写入 16 位的颜色数据到屏幕。
void Lcd_Pushu16(u16 color);
//写入多个字节的颜色数据。
void Lcd_Pushs(u16 *colors, u32 len);
//结束数据写入，关闭片选引脚。
void Lcd_PushEnd();
#endif
