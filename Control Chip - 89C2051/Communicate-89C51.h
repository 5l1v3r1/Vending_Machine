#ifdef Communicate_H
void init_com(void);
void send(int cmd);
int  recv(void);
#endif

#include <reg51.h>
/*

   ��ʱ����ֵ=256-(fosc*(SMOD+1)/384*������)
			 =256-(24000000*(0+1)/384*9600)	 <- ��ʹ24MHz������
			 =256-6=250
			 =0xFA

*/

void init_com(void)	   // ��ʼ��RXD,TXD
{
	 
 	PCON=0x00;		   // ����˫��������
	SM0=0;			   // ��ʽһͨ��
	SM1=1;
	REN=1;			   // ���Խ���
	TI=0;			   // �����Է���
	TMOD=0x20;		   // ���ö�ʱ��
	TH1=0xfd;	       // ��Ӧ���������ֵ���㷽������ֵX=(256��11059200/(12*32*9600)) 
	TL1=0xfd;
	ES=1;
	EA=1;			   // ���������ж�
//	TH1=0xfa;	 /
///	TL1=0xfa;	
	TR1=1;			   // ������ʱ��
}

void send(int cmd)	   // ��������,�������һ����������
{
	SBUF=cmd;		   // д�뵽���ݻ���
	while(TI) TI=0;	   // �ȴ�TI��Ϊ1
}

int recv(void)	       // ��������,�������һ����������
{
	int RecvData=SBUF; // �����ݻ����ж�ȡ����
	RI=0;
	
//	while(RI) RI=0;	   // �ȴ�RI��Ϊ1
	return RecvData;
}