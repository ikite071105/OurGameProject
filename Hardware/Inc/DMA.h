#ifndef DMA_H
#define DMA_H

#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_dma.h"

//USART0_RX -> DMA1 的通道 2 
#define BSP_DMA_RCU            RCU_DMA1  // DMA时钟
#define BSP_DMA                DMA1      // DMA
#define BSP_DMA_CH             DMA_CH2   // DMA通道

#define BSP_DMA_CH_IRQ         DMA1_Channel2_IRQn        // DMA中断
#define BSP_DMA_CH_IRQHandler  DMA1_Channel2_IRQHandler  // DMA中断服务函数

//求传入数组的长度
#define  ARRAYNUM(arr_name)   (uint32_t)(sizeof(arr_name) / sizeof(*(arr_name)))
	
void usart_dma_enable();

extern uint8_t g_transfer_complete;


#endif