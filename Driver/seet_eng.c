#include "seet_eng.h"
static void GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

    GPIO_InitStructure.Pin  = GPIO_Pin_0;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	//初始化
}

// float pwm_val = 90;
static void PWM_config()
{
    PWMx_InitDefine		PWMx_InitStructure;
		
	// 配置PWM5
    PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    =  0;								//PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect  = ENO5P;	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM5, &PWMx_InitStructure);

	// 配置PWMA
    PWMx_InitStructure.PWM_Period   = PERIOD - 1;					//周期时间,   0~65535
    PWMx_InitStructure.PWM_DeadTime = 0;					//死区发生器设置, 0~255
    PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//主输出使能, ENABLE,DISABLE
    PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//使能计数器, ENABLE,DISABLE
    PWM_Configuration(PWMB, &PWMx_InitStructure);			//初始化PWM通用寄存器,  PWMA,PWMB

	PWMB_Prescaler(Prescaler - 1);

    PWM5_SW(PWM5_SW_P20);			//PWM4_SW_P16_P17,PWM4_SW_P26_P27,PWM4_SW_P66_P67,PWM4_SW_P34_P33

	// 初始化PWMA的中断
    // NVIC_PWM_Init(PWMB,DISABLE,Priority_0);
}

void seet_eng_init() {
    GPIO_config();
    PWM_config();
}

void set_seet_eng(float angle) {
    u16 PWM5_Duty;
	if(angle >= 180) angle = 180;
	if(angle <= 0) angle = 0;
	PWM5_Duty = PERIOD * ((angle / 180) * 2 + 0.5) / 20;
	// UpdatePwm(PWMB, &duty);
    PWMB_Duty5(PWM5_Duty);
}