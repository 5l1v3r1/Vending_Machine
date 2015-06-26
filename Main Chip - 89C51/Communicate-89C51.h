#ifdef Communicate_H
void init_com(void);
void send(int cmd);
int recv(void);
#endif
#define No     	 -1
// ����Recv ������������,��˼Ϊû�н��յ�����
#define Busy      0	   // AT89C2051��æ
#define Idle      1	   // AT89C2051����
#define OhNo	  4	   // AT89C2051�ش�����,��˼��û�л���
#define OhYeah	  5	   // AT89C2051�ش�����,��˼���л���
#define WorkStart 10   // ��ʼ����
#define WorkEnd	  11   // �Ѿ�����
// AT89C2051�ش���AT89C51��ֵ
// �������ݵ�ʱ��Ҳ����д���,��������ж�
#include <reg51.h>
/*

   ��ʱ����ֵ=256-(fosc*(SMOD+1)/384*������)
			 =256-(24000000*(0+1)/384*9600)	 <- ��ʹ24MHz������
			 =256-6=250
			 =0xFA

*/

int RecvData=-1;	   // ��Ŵ��ж��н��չ���������,-1Ϊû������

void init_com(void)	   // ��ʼ��RXD,TXD
{
	EA=1;
	ES=1; 			   // �����ж�����������// ��ʹ���ж�����������
 	PCON=0x00;		   // ����˫��������
	SM0=0;			   // ��ʽһͨ��
	SM1=1;
	REN=1;			   // ���Խ���
	TI=0;			   // �����Է���
	TMOD=0x20;		   // ���ö�ʱ��
	TH1=0xfd;	
	TL1=0xfd;	
	TR1=1;			   // ������ʱ��
}

void send(int cmd)	   // ��������,�������һ����������
{
	SBUF=cmd;		   // д�뵽���ݻ���
	while(!TI);		   // �ȴ�TI��Ϊ1
	TI=0;	   
}
/* */
void RecvInterrupt(void) interrupt 4// ���������ж�
{
	if (RI==0) RecvData=No; return; //û�н��յ�����
	switch(SBUF)
	{		
		 case OhNo:		RecvData=OhNo;
		 case OhYeah:	RecvData=OhYeah;
		 case WorkStart:RecvData=WorkStart;
         case WorkEnd:	RecvData=WorkEnd;
	}
	if (RI==1) RI=0;   // ��������֮��,RIΪ0
}
	
int recv(void)	       // ��������,�������һ����������
{
	int rtn=RecvData;
	RecvData=No;
	return rtn;
}