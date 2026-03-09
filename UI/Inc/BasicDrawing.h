#ifndef __BASIC_DRAWING_H__
#define __BASIC_DRAWING_H__

#include "Screen.h"
#include "Font.h"

/*============================================================================
 *                               颜色(RGB565)
 *============================================================================*/
#define COLOR_GOLDEN   (0XFEA0)
#define COLOR_SILVER   (0XC618)
#define COLOR_BRONZE   (0XBB86)
#define COLOR_WHITE    (0xFFFF)
#define COLOR_BLACK    (0x0000)
#define COLOR_BLUE     (0x001F)
#define COLOR_BRED     (0XF81F)
#define COLOR_GRED     (0XFFE0)
#define COLOR_GBLUE    (0X07FF)
#define COLOR_RED      (0xF800)
#define COLOR_MAGENTA  (0xF81F)
#define COLOR_GREEN    (0x07E0)
#define COLOR_CYAN     (0x7FFF)
#define COLOR_YELLOW   (0xFFE0)
#define COLOR_BROWN    (0XBC40)      
#define COLOR_BRRED    (0XFC07)      //棕红
#define COLOR_GRAY     (0X8430)       
#define COLOR_DARKBLUE (0X01CF)   
#define COLOR_LIGHTBLUE  (0X7D7C)  
#define COLOR_GRAYBLUE   (0X5458)   
#define COLOR_LIGHTGREEN (0X841F) 
#define COLOR_LGRAY      (0XC618)      
#define COLOR_LGRAYBLUE  (0XA651) 
#define COLOR_LBBLUE     (0X2B12)
#define COLOR_BODY_R1    (0xFA05)  //蛇身描边
#define COLOR_BODY_R2    (0xFCAE)  //蛇身深红
#define COLOR_BODY_R3    (0xFDF6)  //蛇身浅红
#define COLOR_SCORE_BG   (0xAF78)  //分数栏背景色

/*============================================================================
 *                               颜色模式定义
 *============================================================================*/
typedef enum {
    LCD_COLOR_NORMAL = 0,    // 正常
    LCD_COLOR_REVERSED = 1   // 反色
} LcdColorMode_t;


/*============================================================================
 *                              显存操作函数
 *============================================================================*/
//  清空显存，开始新的一帧
void Lcd_NewFrame(void);

// 将显存内容刷新到屏幕
// 调用此函数后，所有绘制内容才会显示
void Lcd_ShowFrame(void);

/*============================================================================
 *                               像素操作函数
 *============================================================================*/

void Lcd_SetPixel(uint16_t x, uint16_t y, uint16_t color);

uint16_t Lcd_GetPixel(uint16_t x, uint16_t y);

/*============================================================================
 *                               图形绘制函数
 *============================================================================*/
//画线：起始坐标：(x1,y1),终点坐标：(x2,y2)
void Lcd_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
//矩形：左上顶点坐标：(x,y)，宽w，高h
void Lcd_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
//实心矩形
void Lcd_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
//圆：圆心：(x0,y0)，半径r
void Lcd_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
//实心圆
void Lcd_DrawFilledCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
//椭圆：轴心(x0,y0)，长半轴a，高半轴b
void Lcd_DrawEllipse(uint16_t x0, uint16_t y0, uint16_t a, uint16_t b, uint16_t color);
//三角形：顶点(xi,yi)
void Lcd_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
//实心三角形
void Lcd_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
//图片
void Lcd_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char *img);

/*============================================================================
 *                               文字显示函数
 *============================================================================*/
void Lcd_ShowChar(uint16_t x, uint16_t y, char ch, uint16_t fc,  uint8_t size);

void Lcd_ShowChineseChar(uint16_t x, uint16_t y, const char *str, uint16_t fc);

void Lcd_ShowString(uint16_t x, uint16_t y, const char *str, uint16_t fc,uint8_t size);

void Lcd_ShowNumber(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint16_t fc,  uint8_t size);

void Lcd_ShowFloat(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint8_t size);


#endif
