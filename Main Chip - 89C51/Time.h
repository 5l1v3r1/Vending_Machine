#ifndef __TIME_H__
#define __TIME_H__
void  init_time0(void);
void  init_time1(void);
void  init_time2(void);
#endif

//  �жϸ�ʽ  void time0(void) interrupt 1
void init_time0(void)
{
	TMOD |= 0x01;	  //ʹ��ģʽ1��16λ��ʱ����ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ��		     
 	TH0=0x00;	      //������ֵ������ʹ�ö�ʱ�����ֵ��0��ʼ����һֱ��65535�����
 	TL0=0x00;
 	EA=1;             //���жϴ�
 	ET0=1;            //��ʱ��T0�����ж�
 	TR0=1;            //��ʱ��T0��ʼ����
}
//  �жϸ�ʽ  void time1(void) interrupt 3
void init_time1(void)
{
	TMOD |= 0x10;		     
 	TH1=0x00;	      
 	TL1=0x00;
 	EA=1;          
 	ET1=1;            //��ʱ��T1�����ж�
 	TR1=1;            //��ʱ��T1��ʼ����
}
//  �жϸ�ʽ  void time2(void) interrupt 5
void init_time2(void)
{
	TMOD |= 0x10;		     
 	TH2=0x00;	      
 	TL2=0x00;
 	EA=1;          
 	ET2=1;            //��ʱ��T1�����ж�
 	TR2=1;            //��ʱ��T1��ʼ����
}
