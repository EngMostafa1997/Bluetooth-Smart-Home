
#ifndef SENSORS_INTERFACE_H_
#define SENSORS_INTERFACE_H_

/* return temp c*10*/
u16 TEMP_Read(void);

u16 PRESS_Read(void);

u8 POT_Read(ADC_Channel_type CH);



#endif /* SENSORS_INTERFACE_H_ */