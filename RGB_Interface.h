#ifndef RGB_INTERFACE_H_
#define RGB_INTERFACE_H_

typedef struct{
	u8 Red;
	u8 Green;
	u8 Blue;
	}RGB_Colors_Type;

void RGB_color (RGB_Colors_Type colors );
void RGB_color_array (u8* colors );
#endif /* RGB_INTERFACE_H_ */