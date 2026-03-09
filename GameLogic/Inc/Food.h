#ifndef FOOD_H
#define FOOD_H

#include <stdint.h>
#include "BasicDrawing.h"

struct FOOD {
    int x;
    int y;
    int value; // 分数
};

extern struct FOOD food;

void food_init(void);   //初始化
void food_spawn(void);  //刷新

#endif