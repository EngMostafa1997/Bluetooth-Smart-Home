#include "StdTypes.h"
#include "DIO_Interface.h"
#include "MemMap.h"
#include "Timers.h"
#include "LCD_Interface.h"
#include "UltraSonic.h"
#include "UltraSonic_Cfg.h"


#define  F_CPU   8000000
#include <util/delay.h>

static u16 t1,t2,distance,flag=0;

static void ICU_F (void);

void UltraSonic_Init (void)
{
	Timer1_ICU_SetCallBack(ICU_F);
}

void UltraSonic_begin (void)
{
	TCNT1=0;
	Timer1_InputCaptureEdge(RISING);
	DIO_WritePin(UltraSonic_Trigger,HIGH);
	_delay_us(10);
	DIO_WritePin(UltraSonic_Trigger,LOW);
	Timer1_ICU_InterruptEnable();
}

static void ICU_F (void)
{
	if (flag==0)
	{
		t1=ICR1;
		Timer1_InputCaptureEdge(FALLING);
		flag=1;
	}
	else if (flag==1)
	{
		t2=ICR1;
		Timer1_ICU_InterruptDisable();
		distance=(t2-t1)/58;
		flag=0;
	}
}

u16 UltraSonic_distance (void)
{
	return distance;
}

void UltraSonic_Multi_begin ()
{
	TCNT1=0;
	Timer1_InputCaptureEdge(RISING);
	DIO_WritePin(UltraSonic_Trigger,HIGH);
	_delay_us(10);
	DIO_WritePin(UltraSonic_Trigger,LOW);
	Timer1_ICU_InterruptEnable();
}

