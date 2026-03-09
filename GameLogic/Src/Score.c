#include "Score.h"
#include <stdio.h>
#include "BasicDrawing.h"

volatile int score = 0;
void score_init(void) {
    score = 0;
}

void score_update(int value) {
    score += value;
	score_draw();
}

void score_draw(void) {
    char str[16];
    sprintf(str, "%d", score);
    //清除旧分数，绘制新分数
	Lcd_ShowString(122,4,str,COLOR_BLACK,16);
}