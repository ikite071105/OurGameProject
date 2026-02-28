#ifndef __FONT_H__
#define __FONT_H__

typedef enum
{
    fontSize_8 = 8,
    fontSize_12 = 12,
    fontSize_16 = 16,
    fontSize_24 = 24,
    fontSize_32 = 32
} FontSize;

typedef struct
{
     char character[3];
    unsigned char data[32];
} Font16Chinese;

typedef struct
{
     char character[3];
    unsigned char data[24 * 24 / 8];
} Font24Chinese;

typedef struct
{
     char character[3];
    unsigned char data[32 * 32 / 8];
} Font32Chinese;

//16
//16x16 ASCLL
extern const unsigned char font16Char[95][16]; 

#define font16ChineseNUM (10)
//16x16汉字
extern const Font16Chinese font16Chinese[font16ChineseNUM];


//24
//24x24 ASCLL
extern const unsigned char font24Char[95][16*24/8];

#define font24ChineseNUM (20)
//24x24汉字
extern const Font24Chinese font24Chinese[font24ChineseNUM];

//32
//32x32 ASCLL
extern  const unsigned char font32Char[95][16 * 32 / 8];

#define font32ChineseNUM (8)
//32x32汉字
extern const Font32Chinese font32Chinese[font32ChineseNUM];

#endif
