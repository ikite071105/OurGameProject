#ifndef SNAKE_H
#define SNAKE_H

#include <stdint.h>

/*
* 活动区域为240x260，上方240x20为分数栏
* 蛇的一节与一个食物均为20x20的一格
* 相当于活动区域为12x13格
*/
#define LENGTH 12      // 游戏区域长
#define WIDTH  13      // 游戏区域宽
#define CELL_SIZE 20   // 单元格大小，20x20px

// 蛇身方向枚举
typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

struct BODY {
    int x;
    int y;
};

struct SNAKE {
    struct BODY body[LENGTH * WIDTH];
    int size;
	Direction dir;
};

// 全局蛇
extern struct SNAKE snake;

// 坐标映射函数
int map_x(int x);
int map_y(int y);

// 函数接口
void snake_init(void);       // 初始化蛇
void snake_move(void);       // 移动蛇
void snake_grow(void);       // 蛇变长
void snake_erase(void);      // 擦除蛇
void snake_draw(void);       // 绘制蛇

/*============================================================================
 *                               游戏界面绘制
 *============================================================================*/
//蛇头
void darw_snake_head(int x, int y, Direction direction);
//蛇身
void draw_snake_body(int x, int y, int type);
//擦除尾部
void erase_snake_tail(int x,int y);
//食物
void draw_food(int x, int y, int type);
//分数栏
void draw_score_flame();

#endif