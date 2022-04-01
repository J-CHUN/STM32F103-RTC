#include "usart.h"		 

int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
{
	USART_SendData(USART1,(u8)ch);	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

/*******************************************************************************
* 函 数 名         : USART1_Init
* 函数功能		   : USART1初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/ 
void USART1_Init(u32 bound)
{
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //打开时钟
 
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //串口输出PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* 初始化串口输入IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //串口输入PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* 初始化GPIO */
	

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}

/*******************************************************************************
* 函 数 名         : USART1_IRQHandler
* 函数功能		   : USART1中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/ 
static volatile u8  g_usart1_buf[128]={0};
static volatile u32 g_usart1_cnt=0;
	
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 d, i = 0;
	u16 alarm_buf[64]={0};
	u16 rtc_buf[64]={0};
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		d =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		g_usart1_buf[g_usart1_cnt] =  d;
		g_usart1_cnt++;
		
		//设置闹钟
		if(d == 'A'|| g_usart1_cnt>= sizeof(g_usart1_buf))
		{ 		
			char *s = strtok((char *)g_usart1_buf,"- :");  //分割符是- :
			 while(s!=NULL)
			{   
			    alarm_buf[i] = atoi(s);    //2022-1-10 23:50:5A
				i++;
				s = strtok(NULL,"- :");
			}
			RTC_Alarm_Set(alarm_buf[0],alarm_buf[1],alarm_buf[2],alarm_buf[3], alarm_buf[4],alarm_buf[5]); //闹钟设置
			printf("%d-%d-%d %d:%d:%d  设置闹钟成功！\r\n",alarm_buf[0],alarm_buf[1],alarm_buf[2],alarm_buf[3],alarm_buf[4],alarm_buf[5]);
			g_usart1_cnt = 0;
		}
		//设置时间
		else if(d == 'R'|| g_usart1_cnt>= sizeof(g_usart1_buf))
		{ 		
			char *s = strtok((char *)g_usart1_buf,"- :");  //分割符是- :
			 while(s!=NULL)
			{   
			    rtc_buf[i] = atoi(s);    //2022-1-10 23:50:5R
				i++;
				s = strtok(NULL,"- :");
			}
			RTC_Set(rtc_buf[0],rtc_buf[1],rtc_buf[2],rtc_buf[3], rtc_buf[4],rtc_buf[5]); //设置时间
			printf("%d-%d-%d %d:%d:%d  设置时间成功！\r\n",rtc_buf[0],rtc_buf[1],rtc_buf[2],rtc_buf[3],rtc_buf[4],rtc_buf[5]);
			g_usart1_cnt = 0;
		}	
	} 
	USART_ClearFlag(USART1,USART_IT_RXNE);
} 

 



