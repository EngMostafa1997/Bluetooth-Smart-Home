#include "StdTypes.h"
#include "MemMap.h"
#include "Utils.h"
#include "DIO_Interface.h"
#include "MOTOR_Interface.h"
#include "LCD_Interface.h"
#include "KeyPad_Interface.h"
#include "CONVERTER_Interface.h"
#include "ADC_Interface.h"
#include "Sensors_Interface.h"
#include "FireSystem_Interface.h"
#include "TEMP_Filter.h"
#include "EX_Interrupt_Interface.h"
#include "Timers.h"
#include "UltraSonic.h"
#include "EEPROM.h"
#include "UART_Interface.h"
#include "Timers_Services.h"
#include "UART_Services.h"
#include "RGB_Interface.h"
#include "RGB_Cfg.h"
#include "Smart_Interface.h"

#define  F_CPU   8000000
#include <util/delay.h>








int main(void)
{
	DIO_Init();
	LCD_Init();
	//KEYPAD_Init();
	ADC_Init(VREF_AVCC,ADC_SCALER_64);
	GLOBAL_ENABLE();
	UART_Init();
	Smart_Init();
	
	Timer1_OCRA1Mode(OCRA_INVERTING);
	Timer1_OCRB1Mode(OCRA_INVERTING);
	TIMER0_OC0Mode(OCRA_INVERTING);
	
	OCR0=128;
 	OCR1A=0;
 	OCR1B=128;
	 
	Timer1_Init(TIMER1_FASTPWM_FF_TOP_MODE,TIMER1_SCALER_8);
	TIMER0_Init(TIMER0_FASTPWM_MODE,TIMER0_SCALER_8);
	
	

	
	while (1)
	{
		Smart_Home_Runnable();
	}
}