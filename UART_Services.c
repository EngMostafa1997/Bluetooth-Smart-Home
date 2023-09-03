#include "StdTypes.h"
#include "UART_Interface.h"
#include "UART_Services.h"
#include "LCD_Interface.h"


void UART_SendString(u8*str)
{
	u8 i;
	for (i=0;str[i];i++)
	{
		UART_Send(str[i]);
	}	
}


void UART_ReceivePassword(u8*str)
{
	for (u8 i=0 ; i<6 ;i++)
	{
		str[i]=UART_Receive();
	}
}

void UART_ReceiveString2(c8*str)
{
	u8 i=0;
	str[0]=UART_Receive();
	for (;str[i]!='#' && str[i]!=0xBB && str[i]!=0xF5 && i<31;)
	{
		i++;
		str[i]=UART_Receive();
	}
	if (str[i]=='#')
	{
		str[i]=0;
	}
	if (str[i]==0xBB)
	{
		str[0]=0xBB;
	}
	if (str[i]==0xF5)
	{
		str[0]=0xF5;
	}
}

void UART_ReceiveString(u8*str)
{
	u8 i=0;
	str[0]=UART_Receive();
	for (;str[i]!=0x0d;)
	{
		i++;
		str[i]=UART_Receive();
	}
	str[i]=0;
	
}


void UART_SendNumber(u32 num)
{
	UART_Send((u8)num);
	UART_Send((u8)(num>>8));
	UART_Send((u8)(num>>16));
	UART_Send((u8)(num>>24));
}

u32 UART_ReceiveNumber(void)
{
	u8 b0=UART_Receive();
	u8 b1=UART_Receive();
	u8 b2=UART_Receive();
	u8 b3=UART_Receive();
	u32 num=(u32)b0|((u32)b1<<8)|((u32)b2<<16)|((u32)b3<<24);
	return num;
	
}


void UART_SendNumber2(u32 num)
{
	u8*p=(u8*)&num;
	UART_Send(p[0]);
	UART_Send(p[1]);
	UART_Send(p[2]);
	UART_Send(p[3]);
}

u32 UART_ReceiveNumber2(void)
{
	u8* p;
	u32 num;
	p=(u8*)&num;
	 p[0]=UART_Receive();
	 p[1]=UART_Receive();
	 p[2]=UART_Receive();
	 p[3]=UART_Receive();
	
	return num;
	
}


u8 endian_check(void)
{
	u32 num=1;
	u8*p=(u8*)&num;
	if(*p==1)
	{
		return 1;
	}
	else
	{
		return 2;
	}
	
}



u16 endian_convert_u16(u16 num)
{
	return num>>8|num<<8;
}

u32 endian_convert_u32(u32 num)    //0xffee5522   55220000  00000055
{
	
	
	u32 num2;
	
	num2=((num>>24)|(num<<24))|((num>>8)&(0x0000ff00))|((num<<8)&(0x00ff0000));
	//num2=num>>24|num<<24|((num<<16)>>24)<<16|(num<<8)&0x00ff0000;
	return num2;
}


u32 endian_convert_u32_2(u32 num)    //0xffee5522   55220000  00000055
{
	u8*p=(u8*)&num;
	u8 temp;
	temp=p[0];
	p[0]=p[3];
	p[3]=temp;
	temp=p[1];
	p[1]=p[2];
	p[2]=temp;
	return num;
}


void UART_SendStringCheckSum(u8*str)
{
	u8 i,l;
	u16 sum=0;
	for(l=0;str[l];l++)
	{
		sum+=str[l];
	}
	UART_Send(l);
	for (i=0;str[i];i++)
	{
		UART_Send(str[i]);
	}
	UART_Send((u8)sum);
	UART_Send((u8)(sum>>8));
	
}

u8 UART_ReceiveStringCheckSum(u8*str)
{
	u8 l,i,b1,b0;
	u16 sum_rec=0,sum_calc=1;
	l=UART_Receive();
	
	for(i=0;i<l;i++)
	{
		str[i]=UART_Receive();
		sum_calc+=str[i];
	}
	b0=UART_Receive();
	b1=UART_Receive();
	sum_rec=b0|(b1<<8);
		
	if (sum_rec==sum_calc)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/*
static void tx_func(void);
static volatile u8*Str_TX[20];
void UART_SendStringAsynch(u8*str)
{
	UART_Send(str[0]);
	Str_TX=str;
	UART_TX_InterruptEnable();
	UART_TX_SetCallBack(tx_func);
	
}

static void tx_func(void)
{
	static u8 i=1;
	if (Str_TX[i]!=0)
	{
		UART_SendNoBlock(Str_TX[i]);
		i++;
	}
	else
	{
		i=1;
		
		
	}
	
}*/