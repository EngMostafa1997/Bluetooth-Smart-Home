#include "StdTypes.h"
#include "MemMap.h"
#include "RGB_Interface.h"
#include "RGB_Cfg.h"


void RGB_color (RGB_Colors_Type colors )
{
	RedPin=255-colors.Red;
	GreenPin=255-colors.Green;
	BluePin=255-colors.Blue;
}

void RGB_color_array (u8* colors )
{
	RedPin=255-colors[0];
	GreenPin=255-colors[1];
	BluePin=255-colors[2];
}