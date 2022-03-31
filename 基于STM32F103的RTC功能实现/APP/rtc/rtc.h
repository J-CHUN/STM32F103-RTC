#ifndef _rtc_H
#define _rtc_H	 
#include "system.h"

//ʱ��ṹ��
typedef struct 
{
	u8 hour;
	u8 min;
	u8 sec;	
	
	//������������
	u16 w_year;
	u8  w_month;
	u8  w_date;
	u8  week;		 
}_calendar;					 
extern _calendar calendar;	//�����ṹ��

struct SET_ALARM
{
	s8 hour;
	s8 minute;
	u8 week;
};

    
u8 RTC_Init_LSI(void);//��ʼ��RTC,����0,ʧ��;1,�ɹ�;
u8 RTC_Init_LSE(void);
u8 Is_Leap_Year(u16 year);//ƽ��,�����ж�
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u8 RTC_Get(void);         //����ʱ��   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//����ʱ��	
void  get_time(void);
#endif


