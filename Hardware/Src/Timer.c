#include "Timer.h"

//主要用作定时器中断

//TIMER5
#define  BSP_TIMER_RCU    RCU_TIMER5        // 定时器时钟
#define  BSP_TIMER        TIMER5            // 定时器
#define  BSP_TIMER_IRQ    TIMER5_DAC_IRQn   // 定时器中断

#define BSP_TIMER_IRQHandler TIMER5_DAC_IRQHandler // 定时器中断服务函数;

//定时时间 = pre / 200 * per us
//pre（预分频值）  per（周期值）/（自动重装载值）
void basic_timer_config(uint16_t pre,uint16_t per){
	//开启时钟
    rcu_periph_clock_enable(BSP_TIMER_RCU); // 开启定时器的外设时钟
	//APB1总线时钟：通常50MHz
	//CK_TIMERx = 4 x CK_APB1  = 4x50M = 200MHZ 
	//定时器配置为 200MHZ，要进行 4 倍频，选择 RCU_TIMER_PSC_MUL4
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4); // 配置定时器时钟
	timer_deinit(BSP_TIMER);                // 复位定时器
	timer_parameter_struct timer_initpara;  // 定义定时器结构体
	//配置定时器参数
	//对200MHz时钟进行第一次分频
    timer_initpara.prescaler = pre -1;      //  时钟预分频值 0-65535  psc_clk = CK_TIMER / pre
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;      // 边缘对齐：计数到周期值后立即复位
    timer_initpara.counterdirection = TIMER_COUNTER_UP;   // 向上计数：从0数到period值
    timer_initpara.period = per  - 1;                     // 自动重装载值：计数器数到多少产生中断
    //在输入捕获的时候使用  数字滤波器使用的采样频率之间的分频比例 
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;      // 分频因子（用途：输入捕获时，数字滤波器的采样分频
	//DIV1：不分频（此时不起作用
	
    //只有高级定时器才有 配置为x，就重复x+1次进入中断 
    timer_initpara.repetitioncounter = 0;                 // 重复计数器 0-255
	//用途：高级定时器专用，控制中断产生频率//0：每个周期都产生中断
	
    timer_init(BSP_TIMER,&timer_initpara);                // 初始化定时器
	//配置中断优先级
    nvic_irq_enable(BSP_TIMER_IRQ,3,2);             // 设置中断优先级为 3,2断
	//使能中断 
    timer_interrupt_enable(BSP_TIMER,TIMER_INT_UP); // 使能更新事件中断
	//使能定时器
    timer_enable(BSP_TIMER);
}

void BSP_TIMER_IRQHandler(void){
    /* 这里是定时器中断 */
    if(timer_interrupt_flag_get(BSP_TIMER,TIMER_INT_FLAG_UP) == SET)
    {
        timer_interrupt_flag_clear(BSP_TIMER,TIMER_INT_FLAG_UP); // 清除中断标志位
        /* 执行操作 */
        printf("BSP_TIMER_IRQHandler\r\n");
        gpio_bit_toggle(GPIOD,GPIO_PIN_7); // 翻转led
    }
}
