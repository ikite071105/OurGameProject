/*!
    \file    main.c
    \brief   led spark with systick

    \version 2025-07-31, V3.3.2, firmware for GD32F4xx
*/

/*
    Copyright (c) 2025, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "main.h"
#include "Image.h"

/*!
    \brief    main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    systick_config();
	key_init();
	LEDenable();
	Lcd_Init();
	
	SetLED1(true);
	
	//pwm_config(200,10000);  // 不知道是什么总之暂时用不上
	//printf("test1114514\n");
	

    while(1) {
		//当前界面
		switch(current_screen){
			//：主菜单
			case mainscreen:{
				Lcd_NewFrame();
				//当前光标位置
				switch(current_cursor){
				    case START:{
					    Lcd_DrawImage(0,0,240,280,gImage_start);
						break;
					}
					case MODE_SET:{
					    Lcd_DrawImage(0,0,240,280,gImage_mode);
						break;
					}
					case CONFIG:{
						//暂时没有设置功能
					    //Lcd_DrawImage(0,0,240,280,gImage_start);
						break;
					}
				}
				Lcd_ShowFrame();
				if(A_pressed){
					if(current_cursor==0) current_cursor=2;
					else current_cursor--;
					A_pressed = false;
				}
				if(B_pressed){
					if(current_cursor==2) current_cursor=0;
					else current_cursor++;
					B_pressed = false;
				}
				if(R_pressed){
					if(current_cursor==START) current_screen = game_interface;
					//其他两个界面没做
					R_pressed = false;
				}
				if(L_pressed){
				    L_pressed = false;
				}
				break;
			}
			case game_interface:{
				//游戏初始化
				if(!game_has_init){
		            Lcd_NewFrame();
		            game_init();
		            Lcd_ShowFrame();
		        }
				break;
			}
		}
		
		
    }
}

#ifdef GD_ECLIPSE_GCC
/* retarget the C library printf function to the USART, in Eclipse GCC environment */
int __io_putchar(int ch)
{
    usart_data_transmit(EVAL_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));
    return ch;
}
#else
/* retarget the C library printf function to the USART */

#endif /* GD_ECLIPSE_GCC */
