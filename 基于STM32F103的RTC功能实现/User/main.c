#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "rtc.h"

int main()
{
	u8 i=0;	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	SysTick_Init(72);
	LED_Init();
	USART1_Init(9600);
	RTC_Init_LSI(); //�ڲ�����ʱ��
	//RTC_Init_LSE();   //�ⲿ����ʱ��
	printf("��ʼ�����\r\n");
	while(1)
	{
		i++;
		if(i%100==0)
		{
			led1=!led1;
		}
		delay_ms(10);	
	}
}
