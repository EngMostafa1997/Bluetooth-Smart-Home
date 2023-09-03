#include "StdTypes.h"
#include "MemMap.h"
#include "Utils.h"
#include "EEPROM.h"
#include "EEPROM_Private.h"
#include "LCD_Interface.h"


static u16 Arr_Address;
static u8* Arr_data;
static u16 Arr_index;
static u16 Arr_size;

static void (*EE_RDY_Fptr) (void)=NULL_PTR;

void EEPROM_Init(void)
{
	
}

Write_Status_type EEPROM_write(u16 Address, u8 Data)
{
	/*while(EECR & (1<<EEWE));*/
	Write_Status_type status;
	if (Address >= 0 && Address <=1023)
	{
	EEAR = Address;
	EEDR = Data;
	SET_BIT(EECR,EEMWE);
	SET_BIT(EECR,EEWE);
	status=DONE;
	}
	else
	{
		status=OUT_OF_RANGE;
	}
	return status;
}

u8 EEPROM_read(u16 Address)
{
	EEAR = Address;
	SET_BIT(EECR,EERE);
	return EEDR;
}

void EEPROM_InterruptEnable(void)
{
	SET_BIT(EECR,EERIE);
}
void EEPROM_InterruptDisable(void)
{
	CLR_BIT(EECR,EERIE);
}

void EEPROM_EE_RDY_SetCallBack(void(*LocalFptr)(void))
{
	EE_RDY_Fptr=LocalFptr;
}

ISR(EE_RDY_vect)
{
		if(EE_RDY_Fptr!=NULL_PTR)
		{
			EE_RDY_Fptr();
		}
}


static void Array_ISR (void)
{
	if (Arr_index < Arr_size)
	{
		EEPROM_write(Arr_Address,*(Arr_data+Arr_index));
		Arr_index++;
		Arr_Address++;
	}
	else
	{
		EEPROM_InterruptDisable();
		Arr_index=0;
		EEPROM_EE_RDY_SetCallBack(NULL_PTR);
	}
}

Write_Status_type EEPROM_write_array(u16 Address,u8* data,u16 size)
{
	Write_Status_type status;
	if (Address >= 0 && Address+size <=1024)
	{
		Arr_Address=Address;
		Arr_data=data;
		Arr_size=size;
		EEPROM_EE_RDY_SetCallBack(Array_ISR);
		/*EEPROM_InterruptEnable();*/
		EEPROM_write(Address,*data);
		EEPROM_InterruptEnable();
		Arr_index=1;
		Arr_Address++;
		status=DONE;
	}
	else
	{
		status=OUT_OF_RANGE;
	}
	return status;
}

void EEPROM_read_array(u16 Address, u16 size, u8* arr)
{
	for (u16 i=0;i<size;i++)
	{
		*(arr+i)=EEPROM_read(Address+i);
	}
}