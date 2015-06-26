#include <reg51.h>		             // AT89C51�������ſ�
#include <string.h>	              	 // �ַ�����������,�ò�Ͷ�����ж�
#include "delay.h"					 // ��ʱ��,����12MHz����,1ms ����1000��ѭ�����
#include "Key.h"	  				 // ���̿�,��ȡ���µļ���λ��
#include "LCD.h"					 // LCD��ʾ��,�����,��ӡ�Ⱥ���
//#include "Communicate-89C51.h"		 // 89C51ͨ�ſ�,�����˳�ʼ��,���ͽ��պ���
//�������еĲ��԰汾����Ҫ���
//�Զ����	
#define A1 0x01
#define A2 0x02
#define A3 0x03
#define B1 0x11
#define B2 0x12
#define B3 0x13
//AT89C2051��ѯλ�ó���
#define CtrlA1 0x51
#define CtrlA2 0x52
#define CtrlA3 0x53
#define CtrlB1 0x61
#define CtrlB2 0x62
#define CtrlB3 0x63
//AT89C2051����λ�ó���(��AT89C51ʱҲ����)
/*#define Busy      0	   // AT89C2051��æ
#define Idle      1	   // AT89C2051����
#define OhNo	  4	   // AT89C2051�ش�����,��˼��û�л���
#define OhYeah	  5	   // AT89C2051�ش�����,��˼���л���
#define WorkStart 10   // ��ʼ����
#define WorkEnd	  11   // �Ѿ�����
// AT89C2051�ش���AT89C51��ֵ
*/ // ��δ�õ��ĳ���
#define ChipInit    0
//#define ChipRun     1
//#define ShutDown   -1
#define ReadySelect 1
#define ReadyPay	2 
//#define SelectLine  10
//#define SelectList  11
//stat ������״̬
#define NoSelectLine -1	
#define SelectLineA 1
#define SelectLineB 2	
//selline ������״̬
#define NoSelectList NoSelectLine	
#define SelectList1 1
#define SelectList2 2
#define SelectList3 3
#define SelectList4 4	 // δ���õ�,����ʹ��ϵ1-3��
//sellist ������״̬
#define true  1
#define false 0
//boolean ����,C51 û�ж��尡..
/*
 void sleep(int ms);// ��ʱ����
//delay.h
 int scankey(void); // ��ȡ���µļ���λ��,ȡֵ0-15,���󷵻�-1
//Key.h
 void init_lcd(void);// ��ʼ��1602LCD
 void printlcd(char *str);// ����ַ���LCD
 void clear(void);// ���LCD�ϵ��ַ�
//LCD.h
 void init_com(void);// ��ʼ����Ƭ��ͨ�Žӿ�
 void send(int cmd);// ��������
 int  recv(void);// ��������
//Communicate-89C51.h
//�Զ��庯��   */
sbit key1=P3^2;		 
sbit key2=P3^3;
sbit bep =P3^4;
sbit INT =P3^2;
//���Լ�λ
sbit PortA1=P1^0;
sbit PortA2=P1^1;
sbit PortA3=P1^2;
sbit PortB1=P1^3;
sbit PortB2=P1^4;
sbit PortB3=P1^5;
// �����˿�
sbit ChackPort1=P1^6;
sbit ChackPort2=P1^7;
sbit ChackPort3=P3^0;
sbit ChackPort4=P3^1;
sbit ChackPort5=P3^2;
sbit ChackPort6=P3^3;
// ���˿�
int stat=ChipInit;			// ״̬����
int selline=NoSelectLine;	// ѡ�����  (Ĭ��Ϊ-1,��û��ѡ��)
int sellist=NoSelectList;	// ѡ�����
int money=0;				// Ͷ����	 (Ĭ��Ϊ 0,��û��Ͷ��)
//Ҫʹ�õı���

void CtrlPort(int PortIndex)	  	 // ���ƶ˿�
{
  switch(PortIndex)	                 // ��ȡ���յ�����Ϣ
  {
	  case CtrlA1: PortA1=0;sleep(2000);PortA1=1; 	// �ж���Ϣ,��ʼ����ÿһ���˿� ,�ӳ�3���Ǽ���ֵ
	  case CtrlA2: PortA2=0;sleep(2000);PortA2=1;	
	  case CtrlA3: PortA3=0;sleep(2000);PortA3=1;
	  case CtrlB1: PortB1=0;sleep(2000);PortB1=1;	
	  case CtrlB2: PortB2=0;sleep(2000);PortB2=1;
	  case CtrlB3: PortB3=0;sleep(2000);PortB3=1;	  
  }
}

int ChackPoint(int Point)			  // ������λ����û�л�(ע��,����һ����������)
{
//	 int rcv;						  // AT89C51�������ݱ���
	 EA=0;							  // ��ȫ���ж�,�����Ļ��ڵȴ���ʱ��Ͷ�ҾͲ��Ῠ��	 
/*	 send(Point);					  // ����λ�ô��뵽����оƬ	 
	 while(1)						  // ѭ������
	 {
		 rcv=recv();
		 if (rcv==OhNo)  return false;// û����
		 if (rcv==OhYeah)return true; // ��ѽ
		 sleep(100);				  // �ó����ӳ�һ��,������õ�
	 }*/  //  ��δ�����AT89C51��AT89C2051֮���ͨ����������

	 //ȫ�����,���ڽ��ոߵ�ƽ,���粻����͵�ƽ�Ļ�,IO�˿��Ѿ��ǵ͵�ƽҲ�����жϳɹ�

	 switch(Point)
	 {
		 case A1:EA=1;return (ChackPort1==0)?1:0;
         case A2:EA=1;return (ChackPort2==0)?1:0;
		 case A3:EA=1;return (ChackPort3==0)?1:0;
		 case B1:EA=1;return (ChackPort4==0)?1:0;
		 case B2:EA=1;return (ChackPort5==0)?1:0;
		 case B3:EA=1;return (ChackPort6==0)?1:0;
	 }
	 EA=1;							  // �жϼ�����
	 return -1;
}

void ShowLine(void)				      // ��û�й�����ʱ�Ľ���	(����)
{
	 char optmey=money+48;			  // ���ںϳ�ASCII,����Ҫ����48(ASCII48=����0)
	 char str[10]="Yon coin:";        // ׼���ϳ�һ���µ��ַ���,// ����ַ�,�����û���Ͷ�Ҷ���		 	 
	 clear();
	 sleep(5);
	 printlcd("Plaese select!");	 			
	 strcat(str,&optmey);
	 printlcd(str);
	 selline=NoSelectLine;			  // �ص�������,Ҫ����������
	 sellist=NoSelectList;
	 stat=ReadySelect;				  // �ص�ѡ���е�״̬
}

void ChackPay(void)					  // ��������Ǽ�⵱ǰ��money ���ж���,����HasMoney��Determine
{
			     if(money==0) printlcd("Please coin:0/3");
			else if(money==1) printlcd("Please coin:1/3");
			else if(money==2) printlcd("Please coin:2/3");
			else
			{
				money-=3;		         // money-=3;
		        clear();										 
		        sleep(5);
		        printlcd("Pay success");
				sleep(1000);			     // ��ͣ1��
				clear();
				sleep(5);
				printlcd("Control Ready!");	 // ���ƶ˵�оƬ�Ѿ�׼����

				if(selline==SelectLineA)	 // �ֿ�����
				{
					if(sellist==SelectList1) CtrlPort(CtrlA1);
					if(sellist==SelectList2) CtrlPort(CtrlA2);
				    if(sellist==SelectList3) CtrlPort(CtrlA3);
				}
				else if(selline==SelectLineB)
				{
					if(sellist==SelectList1) CtrlPort(CtrlB1);
					if(sellist==SelectList2) CtrlPort(CtrlB2);
				    if(sellist==SelectList3) CtrlPort(CtrlB3);
				}			
//				while(recv()==WorkEnd) break; // ѭ���ж�AT89C2051ϵ��ϵ�Ѿ�������
				printlcd("Control End!!");    // ���ƶ˵�оƬ�Ѿ����н���
				sleep(1500);
				selline=NoSelectLine;
				sellist=NoSelectList;
				stat=ReadySelect;
				ShowLine();
	        }
}

void Determine(void)				  // ȷ������
{
	   clear();
	   sleep(5);
	   // �ж�ѡ�е���
	   if(selline==NoSelectLine)	  // ��ѡ��
	   {
		        printlcd("Error!");
		    	printlcd("No Select!");
				selline=NoSelectLine;
	            sellist=NoSelectList;  
			    ShowLine();
				return;				  // �д���Ļ������˳��������� �д���ס�����˳�����
	   }
	   if(selline==SelectLineA)       // ѡ��A��	  
	   {
			if(sellist==NoSelectList) // û��ѡ�еĻ�Ҳ��ȷ�� ��ѡ����Ҳ��ȷ��
			{ 
			    printlcd("Error");	  // ��ʾ����
				printlcd("No Select List!");
				selline=NoSelectLine;
	            sellist=NoSelectList; // ���ѡ�и�(��)����
			    ShowLine();
				return;				  // ���׳���ס(��)�˳���������
			}
			if(sellist==SelectList1)  // ѡ���е�(��)��,�ٵ�ȷ��
			{	
			    printlcd("Select Point:A1");
				if(!ChackPoint(A1))	  // �����޶�(û����)��..
				{
					 clear();
					 sleep(5);
					 printlcd("Point:A1 No Goos");
					 selline=NoSelectLine;
	                 sellist=NoSelectList;     // ���ѡ�и�����
					 sleep(1500);
					 ShowLine(); 			   // ���ص�����
					 return;
				}
			}
			if(sellist==SelectList2)
			{	
			    printlcd("Select Point:A2");
				if(!ChackPoint(A2))	  
				{
					 clear();
					 sleep(5);
					 printlcd("Point:A2 No Goos");
					 selline=NoSelectLine;
	                 sellist=NoSelectList;     // ���ѡ�и�����
					 sleep(1500);
					 ShowLine(); 			   // ���ص�����
					 return;
				}
			}
			if(sellist==SelectList3)
			{	
			    printlcd("Select Point:A3");
				if(!ChackPoint(A3))	  
				{
					 clear();
					 sleep(5);
					 printlcd("Point:A3 No Goos");
					 selline=NoSelectLine;
	                 sellist=NoSelectList; // ���ѡ�и�����
					 sleep(1500);
					 ShowLine(); 		   // ���ص�����
					 return;
				}
	  		}
	   }
	   if(selline==SelectLineB)	      // ѡ��B��
	   {
			if(sellist==NoSelectList)
			{ 
			    printlcd("Error");
				printlcd("No Select List!");
				selline=NoSelectLine;
	            sellist=NoSelectList; // ���ѡ�и�����
				ShowLine();
				return;
			}
			if(sellist==SelectList1)
			{ 	
		     	printlcd("Select Point:B1");
				if(!ChackPoint(B1))	  
				{
					 clear();
					 sleep(5);
					 printlcd("Point:B1 No Goos");
					 selline=NoSelectLine;
	                 sellist=NoSelectList; // ���ѡ�и�����
					 sleep(1500);
					 ShowLine(); 			   // ���ص�����
					 return;
				}
			}
			if(sellist==SelectList2)
			{ 
				printlcd("Select Point:B2");
				if(!ChackPoint(B2))	  
				{
					 clear();
					 sleep(5);
					 printlcd("Point:B2 No Goos");
					 selline=NoSelectLine;
	                 sellist=NoSelectList; // ���ѡ�и�����
					 sleep(1500);
					 ShowLine(); 			   // ���ص�����
					 return;
				}
			}
			if(sellist==SelectList3)
			{ 	
				printlcd("Select Point:B3");
			    if(!ChackPoint(B3))	  
				{
					 clear();
					 sleep(5);
					 printlcd("Point:B3 No Goos");
					 selline=NoSelectLine;
	                 sellist=NoSelectList; // ���ѡ�и�����
					 sleep(1500);
					 ShowLine(); 			   // ���ص�����
					 return;
				}
			}
	   }
	   
	   stat=ReadyPay;				  // ѡ���˻�,״̬����ҲҪ��,Ȼ����,��ס���Կ��ƽ��ޱ��û�Ͷ��������ѡ������Ҳ..��....
	   ChackPay();					  // Ҫ���һ��..
	   EA=1;							  // �жϼ�����
}

void Cancel(void)					  // ȡ�����׵�ʱ��Ҫ��������
{
	   clear();
	   sleep(5);
	   selline=NoSelectLine;
	   sellist=NoSelectList;
	   stat=ReadySelect;			  // ��ԭ������ѡ��
	   printlcd("Cancel select!");
	   sleep(1500); 				  // �ӳ�3��
	   ShowLine();
}

void HasMoney(void) interrupt 0		  // Ͷ�����ж�
{
	 sleep(20);						  // �������	 
	 money++;
	 if (money>=9) money=9;			  // ��Ҫ��һЩ���������Ƶ�,9��Ǯ�����ٰ�...
	 if (stat==ReadyPay) ChackPay();EA=1;// ��,����Ա����ϵ(�����)�����׸�(�����׵�)��(ѽ) >_< ...
	 if (stat==ReadySelect)ShowLine();// ��,����Ա������(������)�� -_- ...	
	 while(INT==0);				      // �ȴ�INT(P3^2)��ɸߵ�ƽ
} 

void main()
{
   stat=ChipInit;  // ��ʼ��������״̬
   init_lcd();
   sleep(10);
   printlcd("inited .."); // �����LCD��
   // LCD ��ʼ��
   EA=1;		
   EX0=1;  		  
   IT0=0;          // �½��ش���,Ͷ����ר��
   // Ͷ�����жϳ�ʼ��
//   init_com();   // ��ʼ�����ڽ�������
   // AT89C51ͨ�ų�ʼ��
   printlcd("Machine init OK!");
   sleep(1000);	   // ��ʱ����
   ShowLine();
   stat=ReadySelect;   // ת��״̬���������� 
     
   while(1)
   {
	int k=scankey();                          // �����޼��ϹŸ�Ҳ������,������ΰ���¼��̼�ʧ��(����ϵͨ�綼,��ʵ�ϵȶ��þö��޷�Ӧ��)  
	if(stat==ReadySelect)					  // ����Ͷ�ҽ�������,סϵ�޵����������¸���,����ȡ�����׻�ϵ���Ը�
	{  	    
	 if(k==3)	                              // ����ѡ������ (A��)
	 {
		if(selline!=NoSelectLine) continue;	  // �Ѿ�ѡ����
	    clear();
		sleep(5);
		printlcd("Select Line:A"); 	          // ���,��ѡ����A��
		selline=SelectLineA;
	 }	
	 if(k==7)	                              // ����ѡ������ (B��) 
	 {	 
		if(selline!=NoSelectLine) continue;
	    clear();
		sleep(5);
		printlcd("Select Line:B"); 
		selline=SelectLineB;
	 }
   	 if(k==0) // 1��
	 {
	    if(selline==NoSelectLine) continue;	  // ��δ��ѡ����
	    sellist=SelectList1;
	    printlcd("Select List:1");            // ѡ�����
	 }									      
     if(k==1) // 2��
	 {
		if(selline==NoSelectLine) continue;
		sellist=SelectList2;
		printlcd("Select List:2"); 
	 }
     if(k==2) // 3��
	 {
	    if(selline==NoSelectLine) continue;
	    sellist=SelectList3;
	    printlcd("Select List:3");
	 }
	 if(k==0xc)  Cancel();                    // �Ÿ�ϵ(�����)ȷ����(�����ϵ�*��)
	 if(k==0xe)  Determine();                 // �Ÿ�ϵȡ����(�����ϵ�#��)  
    }
	else if(stat==ReadyPay)
	{
	 if(k==0xe)  Determine();                 // �Ÿ�ϵȡ����(�����ϵ�#��) 
   	}
   } 	 
}
