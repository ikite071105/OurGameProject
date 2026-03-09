#include "Snake.h"
#include "Food.h"    // 食物坐标
#include "Score.h"
#include <stdio.h>
#include <stdlib.h>
#include "Image.h"
#include "BasicDrawing.h"

// 蛇全局变量
struct SNAKE snake;
//游戏结束标志
extern volatile int game_over_flag;

// 坐标映射函数
int map_x(int x) { return x * CELL_SIZE; }
int map_y(int y) { return 20 + y * CELL_SIZE; } // 上方20px为分数栏

// 初始化蛇
void snake_init(void) {
    snake.size = 2;
    snake.dir = RIGHT;
    snake.body[0].x = LENGTH / 2;     // 蛇头
    snake.body[0].y = WIDTH / 2;
    snake.body[1].x = LENGTH / 2 - 1; // 蛇身
    snake.body[1].y = WIDTH / 2;
}

// 移动蛇
void snake_move(void) {
	struct BODY head = snake.body[0];
    struct BODY last_tail = snake.body[snake.size - 1]; // 保存尾巴坐标

    // 身体后移
    for (int i = snake.size - 1; i > 0; i--) {
        snake.body[i] = snake.body[i - 1];
    }

    // 更新蛇头
    switch (snake.dir) {
        case UP:    snake.body[0].y--; break;
        case DOWN:  snake.body[0].y++; break;
        case LEFT:  snake.body[0].x--; break;
        case RIGHT: snake.body[0].x++; break;
    }

    // 检查碰撞食物
    if (snake.body[0].x == food.x && snake.body[0].y == food.y) {
        snake_grow();       // 身体变长
        score_update(food.value);  // 分数根据食物
        food_spawn();       // 刷新食物
    } else {
        // 擦掉尾巴
        erase_snake_tail(map_x(last_tail.x), map_y(last_tail.y));
    }
	
	// 撞自己
    for (int i = 1; i < snake.size; i++) {
        if (head.x == snake.body[i].x && head.y == snake.body[i].y) {
            // 游戏结束
			game_over_flag = 1;
            // ?接下来怎么处理
        }
    }
}

// 蛇身体变长
void snake_grow(void) {
    if (snake.size < LENGTH * WIDTH) {
        snake.size++;
    }
}

//擦除蛇
void snake_erase(){
	for (int i = 0; i < snake.size; i++) {
        int x = map_x(snake.body[i].x);
        int y = map_y(snake.body[i].y);

        Lcd_DrawFilledRectangle(x,y,20,20,COLOR_WHITE);
    }
}

// 绘制蛇
void snake_draw(void) {
    for (int i = 0; i < snake.size; i++) {
        int x = map_x(snake.body[i].x);
        int y = map_y(snake.body[i].y);

        if (i == 0) {
            // 蛇头
            darw_snake_head(x, y, snake.dir);
        } else if (i % 2) {
            draw_snake_body(x, y,1);
        } else {
            draw_snake_body(x, y,0);
        }
    }
}


/*============================================================================
 *                               游戏界面绘制
 *============================================================================*/
//头/身都先只做一个颜色
//蛇头
void darw_snake_head(int x, int y, Direction direction){
    switch(direction){
		case UP:
			Lcd_DrawImage(x,y,20,20,gImage_head_up);
		    break;
		case DOWN:
			Lcd_DrawImage(x,y,20,20,gImage_head_down);
		    break;
		case LEFT:
			Lcd_DrawImage(x,y,20,20,gImage_head_left);
		    break;
		case RIGHT:
			Lcd_DrawImage(x,y,20,20,gImage_head_right);
		    break;
	}
}

//蛇身的一节
void draw_snake_body(int x, int y, int type){
	if(type){
    Lcd_DrawFilledRectangle(x,y,20,20,COLOR_BODY_R2);  //深
	Lcd_DrawRectangle(x,y,20,20,COLOR_BODY_R1);
	}
	else{
    Lcd_DrawFilledRectangle(x,y,20,20,COLOR_BODY_R3);  //浅
	Lcd_DrawRectangle(x,y,20,20,COLOR_BODY_R1);
	}
}

//擦除尾部
void erase_snake_tail(int x,int y){
	Lcd_DrawFilledRectangle(x,y,20,20,COLOR_WHITE);
}

//食物
void draw_food(int x, int y, int type){
    switch(type){
        case 1:
            Lcd_DrawImage(x, y, 20, 20, gImage_food1);   //星星
            break;
        case 2:
            Lcd_DrawImage(x, y, 20, 20, gImage_food2);   //爱心
            break;
        case 3:
            Lcd_DrawImage(x, y, 20, 20, gImage_food3);   //钻石
            break;
    }
}

//分数栏
void draw_score_flame(){
    Lcd_DrawImage(100,0,58,20,gImage_score);
}