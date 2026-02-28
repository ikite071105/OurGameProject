#include "LED.h"

//LED初始化
void LEDenable(){
	//使能时钟
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOD);
	rcu_periph_clock_enable(RCU_GPIOE);
	rcu_periph_clock_enable(RCU_GPIOG);
	//配置GPIO
	gpio_mode_set(GPIOD,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_7);             //输出
	gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_7);   //推挽
	gpio_mode_set(GPIOE,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_3);             //输出
	gpio_output_options_set(GPIOE,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_3);   //推挽
	gpio_mode_set(GPIOG,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_3);             //输出
	gpio_output_options_set(GPIOG,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_3);   //推挽
	gpio_mode_set(GPIOA,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_5);             //输出
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_5);   //推挽
}

//设置第一个灯的状态
void SetLED1(bool a){
	if(a){
		gpio_bit_write(GPIOE,GPIO_PIN_3,SET);
	}else{
		gpio_bit_write(GPIOE,GPIO_PIN_3,RESET);
	}
}

//设置第二个灯的状态
void SetLED2(bool a){
	if(a){
		gpio_bit_write(GPIOD,GPIO_PIN_7,SET);
	}else{
		gpio_bit_write(GPIOD,GPIO_PIN_7,RESET);
	}
}

//设置第三个灯的状态
void SetLED3(bool a){
	if(a){
		gpio_bit_write(GPIOG,GPIO_PIN_3,SET);
	}else{
		gpio_bit_write(GPIOG,GPIO_PIN_3,RESET);
	}
}

//设置第四个灯的状态
void SetLED4(bool a){
	if(a){
		gpio_bit_write(GPIOA,GPIO_PIN_5,SET);
	}else{
		gpio_bit_write(GPIOA,GPIO_PIN_5,RESET);
	}
}