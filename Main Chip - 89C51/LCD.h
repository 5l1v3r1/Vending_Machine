#ifdef LCD_H
#define LCD_H

void init_lcd(void);
void printlcd(char *str);
void clear(void);
void crlf(void);
#endif

/*

�Ÿ���Ĭ��ϵ�� 
P2�����������
P1^0 
P1^1 ��д���ݶ�
P1^2 ʹ�ܶ�

*/
#include <reg51.h>
		 
sbit lcdrs=P1^0;
sbit lcdrw=P1^1;
sbit lcden=P1^2; 
void clear(void);
void crlf(void);

void write_com(char com)		//д�����
{
	lcdrs=0;
	P2=com;	
	lcden=1;
	sleep(5);						 			  
	lcden=0;
}

void write_date(char date)		//д���ݺ���
{
	lcdrs=1;
	P2=date;
	lcden=1;
	sleep(5);
	lcden=0;
}

void init_lcd()					//��ʼ������
{	
	lcdrw=0;
	write_com(0x0f);
	write_com(0x38);			//��ʾģʽ���ã�Ĭ��Ϊ0x38,���ñ䡣
	clear();		        	//��ʾ���������ϴε����������Ĭ��Ϊ0x01. 
	write_com(0x80);			//���ó�ʼ������ָ�룬���ڶ�ָ��Ĳ�������е�
}

void printlcd(char *str)					//��ʾ����
{
  int i;
	for(i=0;i<16;i++)
	{
	   	if (str[i]=='\0') break;		
		write_date(str[i]);
	}
	crlf();	
}

void clear(void)
{
    write_com(0x01);	
}

void crlf(void)
{
	write_com(0x80+0x40); 		//��������ָ�룬���ַ�����
	sleep(5);
}


