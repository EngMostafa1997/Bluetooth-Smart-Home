#include "StdTypes.h"
#include "ADC_Interface.h"
#include "Sensors_Cfg.h"
#include "Sensors_Interface.h"

u16 TEMP_Read(void)
{
	u16 volt=ADC_VoltRead(LM35_CH);
	u16 temp;
	
	temp=volt;
	
	return temp;
	
}

u16 PRESS_Read(void)
{
	u16 adc=ADC_Read(NPX4115_CH);
	
	u16 press= ((((u32)1000*(adc-55))+460)/921)+150;
	
	return press;
	
}


u8 POT_Read(ADC_Channel_type CH)
{
	u16 read=ADC_Read(CH);
	u8 percentage=(((u32)100*read)+512)/1024;
	return percentage;
}

u8 POT_Read_Temp(void)
{
	u16 read=ADC_Read(CH_0);
	u8 percentage=(((u32)100*read)+512)/1024;
	return percentage;
}