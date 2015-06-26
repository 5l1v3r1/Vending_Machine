//#include <AT892051.h>
#include <reg51.h>			   // ����2K��������,�ް취��
#include "Communicate-89C51.h"
#define A1 0x01
#define A2 0x02
#define A3 0x03
#define B1 0x11
#define B2 0x12
#define B3 0x13
// ��ѯλ�ó���
#define CtrlA1 0x51
#define CtrlA2 0x52
#define CtrlA3 0x53
#define CtrlB1 0x61
#define CtrlB2 0x62
#define CtrlB3 0x63
// ����λ�ó���
#define Busy      0	   // AT89C2051��æ
#define Idle      1	   // AT89C2051����
#define OhNo	  4	   // ���ϵ�����..
#define OhYeah	  5	   // �Ļ����ϵ�
#define WorkStart 10   // ��ʼ����
#define WorkEnd	  11   // �Ѿ�����
// AT89C2051�ش���AT89C51��ֵ
#define true    1
#define false   0
// Boolean����
#define Working 1
#define Readly	0
// stat ״̬����ȡֵ
/* ԭ���Ƕ����AT89C2051��
sbit PortA1=P1^7;
sbit PortA2=P1^6;
sbit PortA3=P1^5;
sbit PortB1=P1^4;
sbit PortB2=P1^3;
sbit PortB3=P1^2;
// �����Ƶ����
sbit ChackPortA1=P1^1;
sbit ChackPortA2=P1^0;
sbit ChackPortA3=P3^2;
sbit ChackPortB1=P3^3;
sbit ChackPortB2=P3^4;
sbit ChackPortB3=P3^5;
// ����Ÿ�λ��ϵ��ϵ��Ҳ����(ͨ�����������ƽ)
*/
sbit PortA1=P2^1;
sbit PortA2=P2^2;
sbit PortA3=P2^3;
sbit PortB1=P2^4;
sbit PortB2=P2^5;
sbit PortB3=P2^6;

sbit ChackPortA1=P1^0;
sbit ChackPortA2=P1^1;
sbit ChackPortA3=P1^2;
sbit ChackPortB1=P1^3;
sbit ChackPortB2=P1^4;
sbit ChackPortB3=P1^5;

int  stat=Readly;

void   CtrlPort (int PortIndex);
int    ChackPort(int PortIndex);
void   init_com(void);
void   send(int cmd);
int    recv(void);

void Delay(int ms)
{
   int a,b;
   for(a=1;a<=2*ms;a++)
      for(b=1;b<=1000;b++);  
}

void RecvMessageInterrupt(void) interrupt 4
{
	 int rcv=recv();
          if(rcv==A1)		 // ��˵��if д�����Ĵ���ռ��switch С
		  {
			       if (ChackPort(A1)) send(OhYeah);
				   else               send(OhNo);
          }
		  else if(rcv==A2)
		  {
			       if (ChackPort(A2)) send(OhYeah);
				   else               send(OhNo);
          }
		  else if(rcv==A3)
	      {
			       if (ChackPort(A3)) send(OhYeah);
				   else               send(OhNo);
          }
		  else if(rcv==B1)
		  {
			       if (ChackPort(B1)) send(OhYeah);
				   else               send(OhNo);
          }
		  else if(rcv==B2)
		  {
			       if (ChackPort(B2)) send(OhYeah);
				   else               send(OhNo);
          }
		  else if(rcv==B3)
		  {
			       if (ChackPort(B3)) send(OhYeah);
				   else               send(OhNo);
          }
		  else if(rcv==CtrlA1)
		  {
		  send(WorkStart);
		  Delay(1000);
		  CtrlPort(A1);
		  Delay(1000);
		  send(WorkEnd);
          }
		  else if(rcv==CtrlA2)
		  {
		  send(WorkStart);
		  Delay(1000);
		  CtrlPort(A2);
		  Delay(1000);
		  send(WorkEnd);
		  }
		  else if(rcv==CtrlA3)
		  {
		  send(WorkStart);
		  Delay(1000);
		  CtrlPort(A3);
		  Delay(1000);
		  send(WorkEnd);
		  }
		  else if(rcv==CtrlB1)
		  {
		  send(WorkStart);
		  Delay(1000);
		  CtrlPort(B1);
		  Delay(1000);
		  send(WorkEnd);
		  }
		  else if(rcv==CtrlB2)
		  {
		  send(WorkStart);
		  Delay(1000);
		  CtrlPort(B2);
		  Delay(1000);
		  send(WorkEnd);
		  }
		  else if(rcv==CtrlB3)
		  {
		  send(WorkStart);
		  Delay(1000);
		  CtrlPort(B3);
		  Delay(1000);
		  send(WorkEnd);
		  }
		  else{}
}

void main()
{
	init_com();				      	// ��ʼ��RXD,TXD
	while(1);						// ��ѭ��,��Ҫ���ô����ж�������
}

void CtrlPort(int PortIndex)		// ���ƶ˿�
{
  switch(PortIndex)	                // ��ȡ���յ�����Ϣ
  {
	  case A1: PortA1=0;Delay(3000);PortA1=1; 	// �ж���Ϣ,��ʼ����ÿһ���˿� ,�ӳ�3���Ǽ���ֵ
	  case A2: PortA2=0;Delay(3000);PortA2=1;	
	  case A3: PortA3=0;Delay(3000);PortA3=1;
	  case B1: PortB1=0;Delay(3000);PortB1=1;	
	  case B2: PortB2=0;Delay(3000);PortB2=1;
	  case B3: PortB3=0;Delay(3000);PortB3=1;	  
  }
}

int ChackPort(int PortIndex)        // �ж�����˿��Ƿ��л�����
{
  switch(PortIndex)	
  {
	  case A1:	return (ChackPortA1==0)?true:false;	
	  case A2:	return (ChackPortA2==0)?true:false;		
	  case A3:	return (ChackPortA3==0)?true:false;	
	  case B1:	return (ChackPortB1==0)?true:false;		
	  case B2: 	return (ChackPortB2==0)?true:false;	
	  case B3:	return (ChackPortB3==0)?true:false;	  
  }
  return false;
}