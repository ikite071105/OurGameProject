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
	usart_init();
	key_init();
	LEDenable();
	Lcd_Init();
	
	usart_dma_enable();
	
	SetLED1(true);
	
	//pwm_config(200,10000);  // 忘了是什么总之暂时用不到

    while(1) {
		// 返回收到的串口信息
		if(g_recv_complete_flag == SET){
            g_recv_complete_flag = RESET;
            printf("DMA Received: %s\r\n", g_recv_buff);
            printf("Length: %d\r\n", g_recv_length);
    
            g_recv_length = 0;
        }
		
		//CESHI
    Lcd_NewFrame();
		Lcd_DrawFilledRectangle(0,0,240,280,COLOR_WHITE);
    Lcd_DrawImage(10,10,20,20, gImage_food1);
		Lcd_DrawImage(10,30,20,20, gImage_food2);
		Lcd_DrawImage(10,50,20,20, gImage_food3);
		Lcd_DrawFilledRectangle(10,170,20,20,COLOR_BODY_R2);
		Lcd_DrawRectangle(10,170,20,20,COLOR_BODY_R1);
		Lcd_DrawFilledRectangle(30,170,20,20,COLOR_BODY_R3);
		Lcd_DrawRectangle(30,170,20,20,COLOR_BODY_R1);
		Lcd_DrawImage(50,170,20,20,gImage_head_right);
	//Lcd_ShowString(20,100,"贪吃蛇#$a1A",COLOR_YELLOW,32);
	//Lcd_ShowString(20,150,"草履虫/;89yG",COLOR_LIGHTGREEN,24);
	//Lcd_ShowString(20,200,"游戏",COLOR_DARKBLUE,16);
    // 显示结束，刷新屏幕
    Lcd_ShowFrame();
		//CESHI
		
		
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
