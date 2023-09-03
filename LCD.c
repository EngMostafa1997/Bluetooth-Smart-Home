#include "StdTypes.h"
#include "DIO_Interface.h"
#include "LCD_Cfg.h"
#include "LCD_Interface.h"
#include "UTILS.h"
#define  F_CPU   8000000
#include <util/delay.h>


#if LCD_MODE==_8_BIT

void WriteIns(u8 ins)
{
	DIO_WritePin(RS,LOW);
	DIO_WritePort(LCD_PORT,ins);
	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	_delay_ms(1);
	
}

static void WriteData(u8 data)
{
	DIO_WritePin(RS,HIGH);
	DIO_WritePort(LCD_PORT,data);
	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	_delay_ms(1);
	
}

void LCD_Init(void)
{
	_delay_ms(50);
	//0x38;
	WriteIns(0x38);
	WriteIns(0x0c);//0x0e, 0x0f
	WriteIns(0x01);//clear screen
	_delay_ms(1);
	WriteIns(0x06);
}

#else
void WriteIns(u8 ins)
{
	DIO_WritePin(RS,LOW);
	DIO_WritePin(D7,READ_BIT(ins,7));
	DIO_WritePin(D6,READ_BIT(ins,6));
	DIO_WritePin(D5,READ_BIT(ins,5));
	DIO_WritePin(D4,READ_BIT(ins,4));
	
	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	_delay_ms(1);
	DIO_WritePin(D7,READ_BIT(ins,3));
	DIO_WritePin(D6,READ_BIT(ins,2));
	DIO_WritePin(D5,READ_BIT(ins,1));
	DIO_WritePin(D4,READ_BIT(ins,0));
	
	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	_delay_ms(1);
	
}

static void WriteData(u8 data)
{
	DIO_WritePin(RS,HIGH);
	DIO_WritePin(D7,READ_BIT(data,7));
	DIO_WritePin(D6,READ_BIT(data,6));
	DIO_WritePin(D5,READ_BIT(data,5));
	DIO_WritePin(D4,READ_BIT(data,4));
	
	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	_delay_ms(1);
	DIO_WritePin(D7,READ_BIT(data,3));
	DIO_WritePin(D6,READ_BIT(data,2));
	DIO_WritePin(D5,READ_BIT(data,1));
	DIO_WritePin(D4,READ_BIT(data,0));
	
	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	_delay_ms(1);
}

void LCD_Init(void)
{
	_delay_ms(50);
	//0x38;
	WriteIns(0x02);
	WriteIns(0x28);
	WriteIns(0x0c);//0x0e, 0x0f
	WriteIns(0x01);//clear screen
	_delay_ms(1);
	WriteIns(0x06);
}
#endif

void LCD_GoTo(u8 line,u8 cell)
{
	if (line==0)
	{
		WriteIns(0x80+cell);
	}
	else if (line==1)
	{
		WriteIns(0x80+0x40+cell);
	}
}

void LCD_Clear(void)
{
	WriteIns(0x01);//clear screen
	_delay_ms(1);
}

void LCD_WriteChar(u8 ch)
{
	WriteData(ch);
}


void LCD_WriteString(c8*str)
{
	u8 i;
	for (i=0;str[i] && i<32;i++)
	{
		LCD_WriteChar(str[i]);
		if (i==15)
		{
			LCD_GoTo(1,0);
		}
	}
	
}

void LCD_WriteNumber(u32 num) //125
{
	c8 str[20];
	u8 i=0;
	s8 j;
	if (num==0)
	{
		LCD_WriteChar('0');
		return ;
	}
// 	if (num<0)
// 	{
// 		LCD_WriteChar('-');
// 		num=num*(-1);
// 	}
	while(num)
	{
		str[i]=num%10 +'0';
		i++;
		num=num/10;
	}
	for (j=i-1;j>=0;j--)
	{
		LCD_WriteChar(str[j]);
	}
}


void LCD_Write_4D(u16 num) //087
{
	LCD_WriteChar( ((num%10000)/1000)+'0');
	LCD_WriteChar( ((num%1000)/100)+'0');
	LCD_WriteChar( ((num%100)/10)+'0');
	LCD_WriteChar( ((num%10)/1)+'0');
}

void LCD_WriteBinary(u32 num)
{
	s8 i,flag=0,read;
	if (num>65535)
	{
		LCD_WriteString("Can't Display");
		return;
	}
	for(i=15;i>=0;i--)
	{
		read=READ_BIT(num,i);
		if (read==1 && flag==0)
		{
			flag=1;
		}
		if (flag==1)
		{
			LCD_WriteChar(read+'0');
		}
	}
}


void LCD_WriteStringGoto(u8 line ,u8 cell,c8*str)
{
	LCD_GoTo(line,cell);
	LCD_WriteString(str);
}

void LCD_ClearPosition(u8 line ,u8 cell,u8 NoCells)
{
	LCD_GoTo(line,cell);
	for (u8 i=0;i<NoCells;i++)
	{
		LCD_WriteChar(' ');
	}	
}

void LCD_WriteHex(u32 num)
{
	c8 str[16]={0};
	u8 i=0,hex;
	u32 n=num;
	
	while(n)
	{
// 		LCD_WriteNumber(n);
// 		LCD_WriteChar('.');
		hex=n&0x0F;
		if (hex>=0 && hex<=9)
		{
			str[i]=hex+'0';
		}
		else if (hex >=10 && hex<=15)
		{
			str[i]=(hex-10)+'A';
		}
		n>>=4;
		i++;
	}
	for (s8 j=i-1;j>=0;j--)
	{
		LCD_WriteChar(str[j]);
	}
}