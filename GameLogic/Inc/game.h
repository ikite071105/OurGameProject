#ifndef GAME_H
#define GAME_H

//游戏界面
typedef enum{
	mainscreen,
	game_interface,
}Current_Screen;

//主界面选项
typedef enum{
	CONFIG,   //0
	START,    //1
	MODE_SET, //2
}MainScreen_Cursor;

void game_init(void);
void game_update(void);

extern volatile int game_has_init;
extern volatile int game_over_flag;
//初始界面为主菜单
extern volatile Current_Screen current_screen;

//初始光标在游戏开始
extern volatile MainScreen_Cursor current_cursor;

#endif