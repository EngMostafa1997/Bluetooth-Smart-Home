#ifndef EEPROM_H_
#define EEPROM_H_

typedef enum{
	DONE,
	OUT_OF_RANGE
	}Write_Status_type;



void EEPROM_Init(void);
Write_Status_type EEPROM_write(u16 Address, u8 Data);
u8 EEPROM_read(u16 Address);

void EEPROM_InterruptEnable(void);
void EEPROM_InterruptDisable(void);
void EEPROM_EE_RDY_SetCallBack(void(*LocalFptr)(void));
Write_Status_type EEPROM_write_array(u16 Address,u8* data,u16 size);
void EEPROM_read_array(u16 Address, u16 size, u8* arr);

#endif /* EEPROM_H_ */