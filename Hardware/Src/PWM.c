#include "PWM.h"

/* PA5  TIMER1_CH0 */
#define BSP_PWM_RCU      RCU_GPIOA
#define BSP_PWM_PORT     GPIOA
#define BSP_PWM_PIN      GPIO_PIN_5

/* TIMER */
#define BSP_PWM_TIMER_RCU  RCU_TIMER1  // 定时器时钟
#define BSP_PWM_TIMER      TIMER1      // 定时器
#define BSP_PWM_CHANNEL   TIMER_CH_0  //定时器通道

//呼吸灯
void pwm_gpio_config(void)
{
    /* 开启时钟 */
    rcu_periph_clock_enable(BSP_PWM_RCU);
	//配置GPIO模式
    gpio_mode_set(BSP_PWM_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, BSP_PWM_PIN);              //复用
	//配置GPIO输出
    gpio_output_options_set(BSP_PWM_PORT,GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,BSP_PWM_PIN);  //推挽
    /* 配置IO为定时器的通道 */
    gpio_af_set(BSP_PWM_PORT, GPIO_AF_1, BSP_PWM_PIN);
	
}

void pwm_config(uint16_t pre,uint16_t per){
	pwm_gpio_config();
	// 定义定时器结构体
	timer_parameter_struct timer_initpara;
	// 定义输出结构体
	timer_oc_parameter_struct timer_ocintpara;
	/* 开启时钟 */
    rcu_periph_clock_enable(BSP_PWM_TIMER_RCU);           // 开启定时器时钟
    /* CK_TIMERx = 4 x CK_APB1  = 4x50M = 200MHZ */
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4); // 配置定时器时钟（四分频
    timer_deinit(BSP_PWM_TIMER);                          // 复位定时器
    /* 配置定时器参数 */
    timer_initpara.prescaler = pre -1;               //  时钟预分频值  PSC_CLK= 200MHZ / 200 = 1MHZ
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE; // 边缘对齐
    timer_initpara.counterdirection = TIMER_COUNTER_UP;   // 向上计数
    timer_initpara.period = per -1;                     // 周期   T = 10000 * 1MHZ = 10ms  f = 100HZ
    /* 在输入捕获的时候使用  数字滤波器使用的采样频率之间的分频比例 */
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1; // 分频因子
    /* 只有高级定时器才有 配置为x，就重复x+1次进入中断 */
    timer_initpara.repetitioncounter = 0;            // 重复计数器 0-255
    timer_init(BSP_PWM_TIMER,&timer_initpara);       // 初始化定时器
	/* 使能定时器 */
    timer_enable(BSP_PWM_TIMER);
	//输出结构体
	timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;  //通道极性
	timer_ocintpara.outputstate = TIMER_CCX_ENABLE;       //通道状态
	timer_channel_output_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,&timer_ocintpara);
	/* 配置定时器输出功能 */
    timer_channel_output_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,&timer_ocintpara);
	// 配置定时器通道输出脉冲值(占空比50%
	//定时器的周期为 10000-1，则占空比为 5000 / 10000 = 50%。
	timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,5000 - 1);
	// 配置定时器通道输出比较模式
	timer_channel_output_mode_config(BSP_PWM_TIMER,TIMER_CH_0,TIMER_OC_MODE_PWM0);
	// 配置定时器通道输出影子寄存器
	timer_channel_output_shadow_config(BSP_PWM_TIMER,TIMER_CH_0,TIMER_OC_SHADOW_ENABLE);
	//定时器自动重载影子使能
    timer_auto_reload_shadow_enable(BSP_PWM_TIMER);
}

//呼吸灯LED4
void pwm_breathing_lamp(void)
{
    static uint16_t value = 0;
    static uint8_t direct = 0;
    if(direct == 0)
    {
        value += 300;
        if(value > 10000)
        direct = 1; // 改变方向
    }else
    {
        value -= 300;
        if(value <= 0)
        direct = 0; // 改变方向
    }
    timer_channel_output_pulse_value_config(BSP_PWM_TIMER,TIMER_CH_0,value); // 配置定时器通道输出脉冲值
    delay_1ms(50);
}