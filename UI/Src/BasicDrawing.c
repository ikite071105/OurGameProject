#include "BasicDrawing.h"
#include "Image.h"

// 字模库
extern const Font16Chinese font16Chinese[];
extern const Font24Chinese font24Chinese[];
extern const Font32Chinese font32Chinese[];

/*============================================================================
 *                               静态变量
 *============================================================================*/
static uint16_t LCD_GRAM[LCD_W * LCD_H];                     // 显存缓冲区(16位色)

/*============================================================================
 *                               显存操作函数
 *============================================================================*/
void Lcd_NewFrame(void)
{
    // 清屏（黑色）
    memset(LCD_GRAM, 0, sizeof(LCD_GRAM));
}

void Lcd_ShowFrame(void)
{
    uint32_t i;
    
    // 设置显示窗口为整个屏幕
    Lcd_PushStart(0, 0, Lcd_GetW(), Lcd_GetH());
    
    LCD_CS_L;
    LCD_DC_H;              // // 数据模式
    
    //  一次性将整个显存刷到屏幕
    for (i = 0; i < (Lcd_GetW() * Lcd_GetH()); i++) {
        Spi4_Write(LCD_GRAM[i] >> 8);       // 高8位
        Spi4_Write(LCD_GRAM[i] & 0xFF);     // 低8位
    }
    
    LCD_CS_H;
}

/*============================================================================
 *                               像素操作函数
 *============================================================================*/
void Lcd_SetPixel(uint16_t x, uint16_t y, uint16_t color)
{
    // 坐标边界检查
    if (x >= Lcd_GetW() || y >= Lcd_GetH()) {
        return;
    }
    
    // 根据屏幕方向计算实际存储位置
    if (lcd_Orientation == lcdOrientation_Portrait) {
        // 竖屏：正常映射
        LCD_GRAM[y * LCD_W + x] = color;
    } else if (lcd_Orientation == lcdOrientation_APortrait) {
        // 竖屏翻转：需要镜像翻转
        LCD_GRAM[(LCD_H - 1 - y) * LCD_W + (LCD_W - 1 - x)] = color;
    } else if (lcd_Orientation == lcdOrientation_Left) {
        // 横屏：需要旋转90度
        LCD_GRAM[x * LCD_H + (LCD_H - 1 - y)] = color;
    } else {
        // 横屏翻转
        LCD_GRAM[(LCD_W - 1 - x) * LCD_H + y] = color;
    }
}

uint16_t Lcd_GetPixel(uint16_t x, uint16_t y)
{
    // 坐标边界检查
    if (x >= Lcd_GetW() || y >= Lcd_GetH()) {
        return COLOR_BLACK;
    }
    
    // 根据屏幕方向读取实际存储位置
    if (lcd_Orientation == lcdOrientation_Portrait){
        return LCD_GRAM[y * LCD_W + x];
    } else if (lcd_Orientation == lcdOrientation_APortrait){
        return LCD_GRAM[(LCD_H - 1 - y) * LCD_W + (LCD_W - 1 - x)];
    } else if (lcd_Orientation == lcdOrientation_Left){
        return LCD_GRAM[x * LCD_H + y];
    } else {
        return LCD_GRAM[(LCD_W - 1 - x) * LCD_H + (LCD_H - 1 - y)];
    }
}

/*============================================================================
 *                               图形绘制函数
 *============================================================================*/
void Lcd_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    int16_t dx = abs(x2 - x1);
    int16_t dy = abs(y2 - y1);
    int16_t sx = (x1 < x2) ? 1 : -1;
    int16_t sy = (y1 < y2) ? 1 : -1;
    int16_t err = dx - dy;
    int16_t e2;
    
    while (1) {
        Lcd_SetPixel(x1, y1, color);
        
        if (x1 == x2 && y1 == y2) break;
        
        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void Lcd_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    Lcd_DrawLine(x, y, x + w, y, color);         // 上
    Lcd_DrawLine(x, y + h, x + w, y + h, color); // 下
    Lcd_DrawLine(x, y, x, y + h, color);         // 左
    Lcd_DrawLine(x + w, y, x + w, y + h, color); // 右
}

void Lcd_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    uint16_t i, j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            Lcd_SetPixel(x + j, y + i, color);
        }
    }
}

void Lcd_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;
    
    while (x <= y) {
        Lcd_SetPixel(x0 + x, y0 + y, color);
        Lcd_SetPixel(x0 - x, y0 + y, color);
        Lcd_SetPixel(x0 + x, y0 - y, color);
        Lcd_SetPixel(x0 - x, y0 - y, color);
        Lcd_SetPixel(x0 + y, y0 + x, color);
        Lcd_SetPixel(x0 - y, y0 + x, color);
        Lcd_SetPixel(x0 + y, y0 - x, color);
        Lcd_SetPixel(x0 - y, y0 - x, color);
        
        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void Lcd_DrawFilledCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;
    int16_t i;
    
    while (x <= y) {
        for (i = x0 - x; i <= x0 + x; i++) {
            Lcd_SetPixel(i, y0 + y, color);
            Lcd_SetPixel(i, y0 - y, color);
        }
        for (i = x0 - y; i <= x0 + y; i++) {
            Lcd_SetPixel(i, y0 + x, color);
            Lcd_SetPixel(i, y0 - x, color);
        }
        
        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void Lcd_DrawEllipse(uint16_t x0, uint16_t y0, uint16_t a, uint16_t b, uint16_t color)
{
    int16_t x = 0;
    int16_t y = b;
    int32_t a2 = a * a;
    int32_t b2 = b * b;
    int32_t d = b2 + a2 * (1 - 2 * b) / 4;
    
    // 区域1
    while (a2 * y > b2 * x) {
        Lcd_SetPixel(x0 + x, y0 + y, color);
        Lcd_SetPixel(x0 - x, y0 + y, color);
        Lcd_SetPixel(x0 + x, y0 - y, color);
        Lcd_SetPixel(x0 - x, y0 - y, color);
        
        if (d < 0) {
            d += b2 * (2 * x + 3);
        } else {
            d += b2 * (2 * x + 3) + a2 * (2 - 2 * y);
            y--;
        }
        x++;
    }
    
    // 区域2
    d = b2 * (2 * x + 1) * (2 * x + 1) / 4 + a2 * (y - 1) * (y - 1) - a2 * b2;
    while (y >= 0) {
        Lcd_SetPixel(x0 + x, y0 + y, color);
        Lcd_SetPixel(x0 - x, y0 + y, color);
        Lcd_SetPixel(x0 + x, y0 - y, color);
        Lcd_SetPixel(x0 - x, y0 - y, color);
        
        if (d < 0) {
            d += a2 * (3 - 2 * y);
            x++;
            d += b2 * (2 * x + 2);
        } else {
            d += a2 * (3 - 2 * y);
        }
        y--;
    }
}

void Lcd_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{
    Lcd_DrawLine(x1, y1, x2, y2, color);
    Lcd_DrawLine(x2, y2, x3, y3, color);
    Lcd_DrawLine(x3, y3, x1, y1, color);
}

void Lcd_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{
    int16_t x, y;
    int16_t minY = (y1 < y2) ? ((y1 < y3) ? y1 : y3) : ((y2 < y3) ? y2 : y3);
    int16_t maxY = (y1 > y2) ? ((y1 > y3) ? y1 : y3) : ((y2 > y3) ? y2 : y3);
    
    for (y = minY; y <= maxY; y++) {
        int16_t xl = Lcd_GetW(), xr = 0;
        
        // 计算与三条边的交点
        if ((y >= y1 && y <= y2) || (y >= y2 && y <= y1)) {
            int16_t xi = x1 + (int32_t)(x2 - x1) * (y - y1) / (y2 - y1);
            if (xi < xl) xl = xi;
            if (xi > xr) xr = xi;
        }
        if ((y >= y2 && y <= y3) || (y >= y3 && y <= y2)) {
            int16_t xi = x2 + (int32_t)(x3 - x2) * (y - y2) / (y3 - y2);
            if (xi < xl) xl = xi;
            if (xi > xr) xr = xi;
        }
        if ((y >= y3 && y <= y1) || (y >= y1 && y <= y3)) {
            int16_t xi = x3 + (int32_t)(x1 - x3) * (y - y3) / (y1 - y3);
            if (xi < xl) xl = xi;
            if (xi > xr) xr = xi;
        }
        
        // 填充该行
        for (x = xl; x <= xr; x++) {
            Lcd_SetPixel(x, y, color);
        }
    }
}

void Lcd_DrawImage(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const unsigned char img[])
{
    uint16_t i,j;
	uint32_t k=0;
	for(i=0;i<width;i++)
	{
		for(j=0;j<length;j++)
		{
			uint16_t color = (img[k] << 8) | img[k+1];
			//printf(" 0x%X \n",color);
			Lcd_SetPixel(x+j,y+i,color);
			k+=2;
		}
	}
}


/*============================================================================
 *                               静态函数声明
 *============================================================================*/
/**
 * @brief   获取UTF-8编码的字符长度
 * @param   str     UTF-8字符串
 * @return  字符占用的字节数
 */
static uint8_t Lcd_GetUtf8Len(const char *string)
{
    if ((string[0] & 0x80) == 0x00)
    {
        return 1;
    }
    else if ((string[0] & 0xE0) == 0xC0)
    {
        return 2;
    }
    else if ((string[0] & 0xF0) == 0xE0)
    {
        return 3;
    }
    else if ((string[0] & 0xF8) == 0xF0)
    {
        return 4;
    }
    return 0;
}
/**
 * @brief   获取字体点阵数据(ASCII)
 * @param   ch      字符
 * @param   size    字体大小
 * @param   buffer  输出缓冲区
 */
static void Lcd_GetAsciiBitmap(char ch, uint8_t size, uint8_t *buffer)
{
    uint8_t i;
    uint8_t index = ch - ' ';
    
    // 声明外部ASCII字库
    extern const unsigned char font16Char[95][16]; 
    extern const unsigned char font24Char[95][16*24/8];
    extern  const unsigned char font32Char[95][16 * 32 / 8];
    
    switch (size) {
        case 16:
            for (i = 0; i < 16; i++) {
                buffer[i] = font16Char[index][i];
            }
            break;
        case 24:
            for (i = 0; i < 48; i++) {
                buffer[i] = font24Char[index][i];
            }
            break;
        case 32:
            for (i = 0; i < 64; i++) {
                buffer[i] = font32Char[index][i];
            }
            break;
    }
}

/**
 * @brief   获取汉字点阵数据(16x16)
 * @param   str     汉字UTF-8字符串(3字节)
 * @param   buffer  输出缓冲区(32字节)
 * @return  1:找到汉字, 0:未找到
 */
static uint8_t Lcd_Get16ChineseBitmap(const char *str, uint8_t *buffer)
{
    uint16_t k;
    const uint16_t font16ChineseNum = sizeof(font16Chinese)/sizeof(Font16Chinese);
    
    // 查找字模
    for (k = 0; k < font16ChineseNum; k++) {
        // 找到
        if (memcmp(font16Chinese[k].character, str, 3) == 0) {
            // 复制字模数据
            for (uint16_t i = 0; i < 32; i++) {
                buffer[i] = font16Chinese[k].data[i];
            }
            return 1;  // 字模找到了
        }
    }
    return 0;  // 未找到
}

/**
 * @brief   获取汉字点阵数据(24x24)
 * @param   str     汉字UTF-8字符串(3字节)
 * @param   buffer  输出缓冲区(32字节)
 * @return  1:找到汉字, 0:未找到
 */
static uint8_t Lcd_Get24ChineseBitmap(const char *str, uint8_t *buffer)
{
    uint16_t k;
    const uint16_t font24ChineseNum = sizeof(font24Chinese)/sizeof(Font24Chinese);
    
    // 查找字模
    for (k = 0; k < font24ChineseNum; k++) {
        // 找到
        if (memcmp(font24Chinese[k].character, str, 3) == 0) {
            // 复制字模数据
            for (uint16_t i = 0; i < 72; i++) {
                buffer[i] = font24Chinese[k].data[i];
            }
            return 1;  // 字模找到了
        }
    }
    return 0;  // 未找到
}

/**
 * @brief   获取汉字点阵数据(32x32)
 * @param   str     汉字UTF-8字符串(3字节)
 * @param   buffer  输出缓冲区(32字节)
 * @return  1:找到汉字, 0:未找到
 */
static uint8_t Lcd_Get32ChineseBitmap(const char *str, uint8_t *buffer)
{
    uint16_t k;
    const uint16_t font32ChineseNum = sizeof(font32Chinese)/sizeof(Font32Chinese);
    
    // 查找字模
    for (k = 0; k < font32ChineseNum; k++) {
        // 找到
        if (memcmp(font32Chinese[k].character, str, 3) == 0) {
            // 复制字模数据
            for (uint16_t i = 0; i < 128; i++) {
                buffer[i] = font32Chinese[k].data[i];
            }
            return 1;  // 字模找到了
        }
    }
    return 0;  // 未找到
}

/*============================================================================
 *                               文字显示函数
 *============================================================================*/
//ASCII字符
void Lcd_ShowChar(uint16_t x, uint16_t y, char ch, uint16_t fc, uint8_t size)
{
    uint8_t i, j, k;
    uint8_t buffer[64];            // 存放点阵数据
    uint8_t charWidth = size / 2;  // ASCII字符宽度是高度的一半
    uint8_t bytesPerRow = (charWidth + 7) / 8;  // 每行占用的字节数
	//printf("wid: %u,bayePerRow: %u \n",charWidth,bytesPerRow);

    // 获取字符点阵
    Lcd_GetAsciiBitmap(ch, size, buffer);

    // 绘制字符到显存
	switch(size){
		case 16:{
			for (i = 0; i < size; i++) {             // 行循环
                for (j = 0; j < bytesPerRow; j++) {  // 每行的字节循环
                    for (k = 0; k < 8; k++) {        // 每个字节的位循环
                        if (j * 8 + k >= charWidth) break;
                        if (buffer[i * bytesPerRow + j] & (0x80 >> k)) {
                        // 前景色(字符点)
                        Lcd_SetPixel(x + charWidth - (j * 8 + k), y + i, fc);
                        } 
                    } 
                }
            }
			break;}
		case 24:{
    for (i = 0; i < size; i++) {
        for (j = 0; j < bytesPerRow; j++) {

            uint8_t data = buffer[i * bytesPerRow + j];

            for (k = 0; k < 8; k++) {

                uint8_t pixelIndex = j * 8 + k;
                if (pixelIndex >= charWidth)
                    break;

                if (data & (1 << k)) {

                    Lcd_SetPixel(
                        x + pixelIndex,
                        y + i,
                        fc
                    );
                }
            }
        }
    }
    break;
}
		case 32:{
			for (i = 0; i < size; i++) {             // 行循环
                for (j = 0; j < bytesPerRow; j++) {  // 每行的字节循环
                    for (k = 0; k < 8; k++) {        // 每个字节的位循环
                        if (j == 0) {
                        // 左半
                            if (buffer[i * 2 + j] & (0x80 >> k)) {
                            Lcd_SetPixel(x + 7 - k, y + i, fc); 
                            }
                        } else {
                        // 右半
                            if (buffer[i * 2 + j] & (0x80 >> k)) {
                            Lcd_SetPixel(x + 15 - k, y + i, fc);
                            }
                        } 
                    } 
                }
            }
			break;}
	}
}

/**
 * @brief   显示单个16x16汉字
 * @param   x       起始横坐标
 * @param   y       起始纵坐标
 * @param   str     汉字UTF-8字符串(3字节)
 * @param   fc      前景色
 * @param   bc      背景色
 */
void Lcd_Show16ChineseChar(uint16_t x, uint16_t y, const char *str, uint16_t fc)
{
    uint8_t i, j, k;
    uint8_t buffer[32];  // 存放16x16汉字点阵(32字节)
    // 获取汉字点阵
    if (!Lcd_Get16ChineseBitmap(str, buffer)) {
        // 未找到汉字，显示空格(全背景色)
        return;
    }

    // 绘制汉字到显存
    for (i = 0; i < 16; i++) {     // 行循环(16行)
        for (j = 0; j < 2; j++) {  // 每行2个字节
            for (k = 0; k < 8; k++) {  // 每个字节8位
                
                if (j == 0) {
                    // 左半
                    if (buffer[i * 2 + j] & (0x80 >> k)) {
                        Lcd_SetPixel(x + 7 - k, y + i, fc); 
                    }
                } else {
                    // 右半
                    if (buffer[i * 2 + j] & (0x80 >> k)) {
                        Lcd_SetPixel(x + 15 - k, y + i, fc);
                    }
                }
            }
        }
    }
}

/**
 * @brief   显示单个24x24汉字
 * @param   x       起始横坐标
 * @param   y       起始纵坐标
 * @param   str     汉字UTF-8字符串(3字节)
 * @param   fc      前景色
 * @param   bc      背景色
 */
void Lcd_Show24ChineseChar(uint16_t x, uint16_t y, const char *str, uint16_t fc)
{
    uint8_t i, j, k;
    uint8_t buffer[72];  // 存放24x24汉字点阵（24*24/8 = 72字节）

    // 获取汉字点阵
    if (!Lcd_Get24ChineseBitmap(str, buffer)) {
        // 未找到汉字，显示空格（全背景色）
        return;
    }

    uint8_t charWidth = 24;           // 宽度 = 高度
    uint8_t bytesPerRow = 3;          // 每行3个字节

    // 绘制汉字到显存
    for (i = 0; i < 24; i++) {         // 行
        for (j = 0; j < bytesPerRow; j++) { // 每行字节
            uint8_t data = buffer[i * bytesPerRow + j];
            for (k = 0; k < 8; k++) {   // 每位
                uint8_t pixelIndex = j * 8 + k;
                if (pixelIndex >= charWidth) break;

                // 24x24 LSB在左
                if (data & (1 << k)) {
                    Lcd_SetPixel(x + pixelIndex, y + i, fc);
                }
            }
        }
    }
}

/**
 * @brief   显示单个32x32汉字
 * @param   x       起始横坐标
 * @param   y       起始纵坐标
 * @param   str     汉字UTF-8字符串(3字节)
 * @param   fc      前景色
 * @param   bc      背景色
 */
void Lcd_Show32ChineseChar(uint16_t x, uint16_t y, const char *str, uint16_t fc)
{
    uint8_t i, j, k;
    uint8_t buffer[128];  // 存放32x32汉字点阵（32*32/8 = 128字节）

    // 获取汉字点阵
    if (!Lcd_Get32ChineseBitmap(str, buffer)) {
        // 未找到汉字，显示空格（全背景色）
        return;
    }

    uint8_t charWidth = 32;   // 宽度 = 高度
    uint8_t bytesPerRow = 4;  // 每行4个字节

    // 绘制汉字到显存
    for (i = 0; i < 32; i++) {         // 行
        for (j = 0; j < bytesPerRow; j++) { // 每行字节
            uint8_t data = buffer[i * bytesPerRow + j];
            for (k = 0; k < 8; k++) {   // 每位
                uint8_t pixelIndex = j * 8 + k;
                if (pixelIndex >= charWidth) break;

                // 32x32 MSB在左，需要左右分开绘制
                if (data & (0x80 >> k)) {
                    uint16_t px;
                    if(j==0)       px = x + 7 - k;   // 左半
                    else if(j==1)  px = x + 15 - k;  // 右半
                    else if(j==2)  px = x + 23 - k;  // 第三字节
                    else           px = x + 31 - k;  // 第四字节
                    Lcd_SetPixel(px, y + i, fc);
                }
            }
        }
    }
}


/**
 * @brief   显示字符串(自动识别汉字和ASCII)
 * @param   x       起始横坐标
 * @param   y       起始纵坐标
 * @param   str     字符串(UTF-8编码)
 * @param   fc      前景色
 * @param   bc      背景色
 * @param   size    字体大小(汉字固定size x size，ASCII为size/2 x size)
 */
void Lcd_ShowString(uint16_t x, uint16_t y, const char *str, uint16_t fc, uint8_t size)
{
    //printf("To Show: %s\n", str);
    uint16_t i = 0; 
    uint16_t x0 = x;
    uint16_t y0 = y;
    uint8_t utf8Len;
    uint8_t asciiWidth = size / 2;       // ASCII字符宽度
    uint8_t chineseWidth = size;         // 汉字宽度
    
    while (str[i]) {
        utf8Len = Lcd_GetUtf8Len(str + i);
        if (utf8Len == 0)
            break; // 有问题的UTF-8编码
        
        if (utf8Len == 3) {  // 汉字(UTF-8 3字节)
            // 提取并显示汉字
			char chineseChar[4] = {0};
            memcpy(chineseChar, str + i, 3);  // memcpy 更直接，不依赖 null
            chineseChar[3] = '\0';            // 强制 null 结尾
			switch(size){
				case 16:
					Lcd_Show16ChineseChar(x0, y0, chineseChar, fc);
					break;
				case 24:
					Lcd_Show24ChineseChar(x0, y0, chineseChar, fc);
					break;
				case 32:
					Lcd_Show32ChineseChar(x0, y0, chineseChar, fc);
					break;
			}
            
            //printf("Searching for: %s\n", chineseChar);
            // 显示汉字
            
            x0 += chineseWidth;
            i += 3;  // 跳过这3字节
        } else if (utf8Len == 1) {  // ASCII字符
            // 显示ASCII字符
            Lcd_ShowChar(x0, y0, *(str + i), fc, size);
            x0 += asciiWidth;
            i += 1;
        } else {
            // 其他编码跳过
            i += utf8Len;
        }
        
        // 超出屏幕宽度则换行
        if (x0 + ((utf8Len == 3) ? chineseWidth : asciiWidth) >= Lcd_GetW()) {
            x0 = x;
            y0 += (utf8Len == 3) ? 16 : size;  // 汉字换行加16，ASCII加size
        }
    }
}
/**
 * @brief   计算10的幂
 */
static uint32_t Lcd_Pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--) result *= m;
    return result;
}

void Lcd_ShowNumber(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint16_t fc, uint8_t size)
{
    uint8_t i;
    uint8_t temp;
    uint8_t showFlag = 0;
    uint8_t charWidth = size / 2;
    
    for (i = 0; i < len; i++) {
        temp = (num / Lcd_Pow(10, len - i - 1)) % 10;
        
        if (showFlag == 0 && i < (len - 1)) {
            if (temp == 0) {
                Lcd_ShowChar(x + i * charWidth, y, ' ', fc,size);
                continue;
            } else {
                showFlag = 1;
            }
        }
        Lcd_ShowChar(x + i * charWidth, y, temp + '0', fc,size);
    }
}

void Lcd_ShowFloat(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc,uint8_t size)
{
    uint8_t i;
    uint8_t temp;
    uint16_t integer = (uint16_t)num;
    uint16_t decimal = (uint16_t)((num - integer) * 100);
    uint8_t charWidth = size / 2;
    
    // 显示整数部分
    Lcd_ShowNumber(x, y, integer, len - 3, fc, size);
    
    // 显示小数点
    Lcd_ShowChar(x + (len - 3) * charWidth, y, '.', fc, size);
    
    // 显示小数部分(两位)
    for (i = 0; i < 2; i++) {
        temp = (decimal / Lcd_Pow(10, 1 - i)) % 10;
        Lcd_ShowChar(x + (len - 2 + i) * charWidth, y, temp + '0', fc, size);
    }
}

