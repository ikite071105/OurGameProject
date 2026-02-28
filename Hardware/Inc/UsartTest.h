#ifndef USARTTEST_H
#define USARTTEST_H

#include "gd32f4xx.h"
#include "systick.h"
#include <stdbool.h>
#include <stdio.h>

#define BSP_USART_RCU           RCU_USART0
#define BSP_USART_TX_RCU        RCU_GPIOA
#define BSP_USART_RX_RCU        RCU_GPIOA

#define BSP_USART_TX_PORT       GPIOA
#define BSP_USART_TX_PIN        GPIO_PIN_9
#define BSP_USART_RX_PORT       GPIOA
#define BSP_USART_RX_PIN        GPIO_PIN_10
#define BSP_USART_AF            GPIO_AF_7  // 串口是引脚复用功能7

//要使能的串口
#define BSP_USART               USART0
//串口中断
#define BSP_USART_IRQ           USART0_IRQn
//串口中断服务函数
#define BSP_USART_IRQHandler    USART0_IRQHandler

//串口缓冲区的数据长度
#define USART_RECEIVE_LENGTH    4096

void usart_init();
void usart_send_data(uint8_t s);
void usart_send_string(uint8_t *ucstr);

//变量定义
extern uint8_t  g_recv_buff[USART_RECEIVE_LENGTH]; // 接收缓冲区
extern uint16_t g_recv_length;                     // 接收数据长度
extern uint8_t  g_recv_complete_flag;              // 接收完成标志位

#endif