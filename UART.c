#include "StdTypes.h"
#include "UTILS.h"
#include "MemMap.h"
#include "UART_Interface.h"




static void(*UART_RX_Fptr)(void)=NULL_PTR;
static void(*UART_TX_Fptr)(void)=NULL_PTR;


void UART_Init(void)
{
	//baud rate 9600
	UBRRL=51;
	//normal speed
	CLR_BIT(UCSRA,U2X);
	
	
	//frame (stop ,data ,parity) 1 STOP NO PARITY 8 DATA
	
	// ENABLE RX TX
	SET_BIT(UCSRB,RXEN);
	SET_BIT(UCSRB,TXEN);
}

void UART_Send(u8 data)
{
	while(!READ_BIT(UCSRA,UDRE));
	UDR=data;
}

void UART_SendNoBlock(u8 data)
{
	UDR=data;
}

u8 UART_ReceiveNoBlock(void)
{
	return UDR;
}




u8 UART_Receive(void)
{
	while(!READ_BIT(UCSRA,RXC));
	return UDR;
}


u8 UART_ReceivePerodic(u8*pdata)
{
	if (READ_BIT(UCSRA,RXC))
	{
		
		*pdata=UDR;
		return 1;
	}
	return 0;
}



void UART_RX_InterruptEnable(void)
{
	SET_BIT(UCSRB,RXCIE);
}

void UART_RX_InterruptDisable(void)
{
	CLR_BIT(UCSRB,RXCIE);
}

void UART_TX_InterruptEnable(void)
{
	SET_BIT(UCSRB,TXCIE);
}

void UART_TX_InterruptDisable(void)
{
	CLR_BIT(UCSRB,TXCIE);
}

void UART_RX_SetCallBack(void (*LocalFptr)(void))
{
	UART_RX_Fptr = LocalFptr;
}

void UART_TX_SetCallBack(void (*LocalFptr)(void))
{
	UART_TX_Fptr = LocalFptr;
}


ISR(UART_RX_vect)
{
	if (UART_RX_Fptr!=NULL_PTR)
	{
		UART_RX_Fptr();
	}
}

ISR(UART_TX_vect)
{
	if (UART_TX_Fptr!=NULL_PTR)
	{
		UART_TX_Fptr();
	}
}