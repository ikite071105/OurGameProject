#include "game.h"
#include "Snake.h"
#include "Food.h"
#include "Score.h"
#include "BasicDrawing.h"

// 游戏结束标志
volatile int game_over_flag = 0;
volatile int game_has_init = 0;
//初始界面为主菜单
volatile Current_Screen current_screen = mainscreen;
//初始光标在游戏开始
volatile MainScreen_Cursor current_cursor = START;

void game_init(void) {
	Lcd_DrawFilledRectangle(0,0,240,280,COLOR_WHITE);
	//分割线
	Lcd_DrawLine(0,19,240,19,COLOR_BLACK);
    score_init();
	draw_score_flame();
	score_draw();
    snake_init();
	snake_draw();
    food_init();
    game_over_flag = 0;
	game_has_init = 1;
}

//更新游戏状态
void game_update(void) {
    if (game_over_flag){
		//游戏结束，更新当前状态（返回主界面）
	}

    snake_move();
    snake_draw();
}