#include "DMA.h"
#include "UsartTest.h"
#include <string.h>

uint8_t g_transfer_complete = 0;

void usart_dma_enable(void)
{
    // 确保串口已经初始化
    // 1. 使能DMA时钟
    rcu_periph_clock_enable(BSP_DMA_RCU);
    
    // 2. 解除DMA初始化
    dma_deinit(BSP_DMA, BSP_DMA_CH);
    
    // 3. 配置DMA参数
    dma_single_data_parameter_struct dma_init_struct;
    memset(&dma_init_struct, 0, sizeof(dma_init_struct));
    
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(BSP_USART);  // 串口数据寄存器
    dma_init_struct.memory0_addr = (uint32_t)g_recv_buff;           // 内存地址
    dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;               // 外设到内存
    dma_init_struct.number = ARRAYNUM(g_recv_buff);                 // 传输数量
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;       // 外设地址固定
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;        // 内存地址递增
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;    // 8位数据
    dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;      // 非循环模式
    dma_init_struct.priority = DMA_PRIORITY_HIGH;                   // 优先级
    
    // 4. 初始化DMA
    dma_single_data_mode_init(BSP_DMA, BSP_DMA_CH, &dma_init_struct);
    
    // 5. 配置DMA通道映射（重要！）
    dma_channel_subperipheral_select(BSP_DMA, BSP_DMA_CH, DMA_SUBPERI4);
    
    // 6. 使能DMA通道
    dma_channel_enable(BSP_DMA, BSP_DMA_CH);
    
    // 7. 使能串口的DMA接收
    usart_dma_receive_config(BSP_USART, USART_RECEIVE_DMA_ENABLE);
}

//DMA 传输完成中断对应的代码
void BSP_DMA_CH_IRQHandler(void){
    /* 传输完成中断 */
    if(dma_interrupt_flag_get(BSP_DMA, BSP_DMA_CH, DMA_INT_FLAG_FTF))
     {
         dma_interrupt_flag_clear(BSP_DMA, BSP_DMA_CH, DMA_INT_FLAG_FTF);
         g_transfer_complete = SET;  // 数据传输完成
    }
}