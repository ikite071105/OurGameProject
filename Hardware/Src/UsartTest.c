#include "UsartTest.h"
#include "DMA.h"

// USART1_TX   PA9
// USART1_RX   PA10

//变量定义
uint8_t g_recv_buff[USART_RECEIVE_LENGTH];  // 根据实际大小调整
uint16_t g_recv_length = 0;
uint8_t g_recv_complete_flag = 0;

//串口的初始化及配置
void usart_init(){
	//开启时钟
	rcu_periph_clock_enable(BSP_USART_RCU);    // 开启串口时钟
    rcu_periph_clock_enable(BSP_USART_TX_RCU); // 开启端口时钟
    rcu_periph_clock_enable(BSP_USART_RX_RCU); // 开启端口时钟
	//配置GPIO复用功能，把 PA9,PA10 设置为串口功能
    gpio_af_set(BSP_USART_TX_PORT,BSP_USART_AF,BSP_USART_TX_PIN);  //PA9
    gpio_af_set(BSP_USART_RX_PORT,BSP_USART_AF,BSP_USART_RX_PIN);  //PA10
	//配置TX为复用模式 上拉模式
    gpio_mode_set(BSP_USART_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BSP_USART_TX_PIN);
    //配置RX为复用模式 上拉模式 
    gpio_mode_set(BSP_USART_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BSP_USART_RX_PIN);
	//配置TX为推挽输出 50MHZ
    gpio_output_options_set(BSP_USART_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BSP_USART_TX_PIN);
    //配置RX为推挽输出 50MHZ 
    gpio_output_options_set(BSP_USART_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BSP_USART_RX_PIN);
	//串口配置
    usart_deinit(BSP_USART);                        // 复位串口
    usart_baudrate_set(BSP_USART,115200U);          // 设置波特率
    usart_parity_config(BSP_USART,USART_PM_NONE);   // 没有校验位
    usart_word_length_set(BSP_USART,USART_WL_8BIT); // 8位数据位
    usart_stop_bit_set(BSP_USART,USART_STB_1BIT);   // 1位停止位
	//串口使能
	usart_transmit_config(BSP_USART,USART_TRANSMIT_ENABLE); // 使能串口发送
	usart_receive_config(BSP_USART, USART_RECEIVE_ENABLE);  // 使能串口接收
    usart_enable(BSP_USART);                                // 使能串口
	//串口中断配置
	usart_interrupt_enable(BSP_USART, USART_INT_RBNE); // 读数据缓冲区非空中断和过载错误中断（接收每一个字符的数据
	//串口每接收到一个字节就会触发↑
    usart_interrupt_enable(BSP_USART, USART_INT_IDLE); // DLE线检测中断（判断一帧数据是否传输完成
    //配置中断优先级
    nvic_irq_enable(BSP_USART_IRQ, 2, 2);              // 配置中断优先级
}

//串口发送单个字符
void usart_send_data(uint8_t s){
	//发送数据
	usart_data_transmit(BSP_USART,(uint8_t)s);
	//等待数据发送完成
	while(usart_flag_get(BSP_USART,USART_FLAG_TBE) == RESET);
}

//串口发送字符串
void usart_send_string(uint8_t *ucstr)
{
      while(ucstr && *ucstr)  // 地址为空或者值为空跳出
      {
        usart_send_data(*ucstr++);
      }
}

//printf重定向
int fputc(int ch, FILE *f)
{
    usart_data_transmit(BSP_USART, (uint8_t)ch);
    while(RESET == usart_flag_get(BSP_USART, USART_FLAG_TBE)); // 等待发送数据缓冲区标志置位
    return ch;
}

//串口中断服务函数
void BSP_USART_IRQHandler(void)
{
    /*if(usart_interrupt_flag_get(BSP_USART,USART_INT_FLAG_RBNE) != RESET) // 接收缓冲区不为空
    {
         g_recv_buff[g_recv_length++] = usart_data_receive(BSP_USART);  // 把接收到的数据放到缓冲区中
		//usart_data_receive()：读取串口数据寄存器
    }
    if(usart_interrupt_flag_get(BSP_USART,USART_INT_FLAG_IDLE) == SET) // 检测到帧中断
    {   //检测串口是否空闲（IDLE = 线路空闲）
        usart_data_receive(BSP_USART);      // 清除空闲中断标志
		//清除空闲中断标志位（在某些芯片上必须读取才能清除）
        g_recv_buff[g_recv_length] = '\0';  //在字符串末尾补\0
        g_recv_complete_flag = SET;         // 接收完成
		/重新设置DMA传输
        dma_channel_disable(BSP_DMA, BSP_DMA_CH);     // 失能DMA通道
        usart_dma_enable();  // 重新配置DMA进行传输
    } */
    // 只处理空闲中断
    if(usart_interrupt_flag_get(BSP_USART, USART_INT_FLAG_IDLE) == SET)
    {
        // 1. 清除空闲中断标志
        usart_data_receive(BSP_USART);
        
        // 2. 暂停DMA传输
        dma_channel_disable(BSP_DMA, BSP_DMA_CH);
        
        // 3. 计算接收到的数据长度
        uint32_t remaining = dma_transfer_number_get(BSP_DMA, BSP_DMA_CH);
        g_recv_length = ARRAYNUM(g_recv_buff) - remaining;
        
        // 4. 添加字符串结束符
        if(g_recv_length < ARRAYNUM(g_recv_buff))
            g_recv_buff[g_recv_length] = '\0';
        else
            g_recv_buff[ARRAYNUM(g_recv_buff)-1] = '\0';
        
        // 5. 设置完成标志
        g_recv_complete_flag = SET;
        
        // 6. 重新配置DMA（重要！）
        dma_channel_disable(BSP_DMA, BSP_DMA_CH);
        dma_deinit(BSP_DMA, BSP_DMA_CH);
        usart_dma_enable();  // 重新初始化DMA
    }
}