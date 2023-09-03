#include "StdTypes.h"
#include "UART_Interface.h"
#include "UART_Services.h"
#include "LCD_Interface.h"
#include "DIO_Interface.h"
#include "EEPROM.h"
#include "Smart_Interface.h"
#include "Smart_Private.h"
#include "RGB_Interface.h"
#include "RGB_Cfg.h"

#define  F_CPU   8000000
#include <util/delay.h>

u8 mode=0;
u8 Lcd_control_mode=0;
u8 Rgb_mode=0;
u8 uart,c;
c8 str[32];
u8 password[6]={0};
DIO_Pin_type led=PINC0;

void Smart_Init (void)
{
	if (EEPROM_read(0)==1)
	{
		EEPROM_read_array(1,6,password);
	}
}

static Check_Passowrd_type check_password (void)
{
	u8 check[6];
	Check_Passowrd_type answer= Valid;
	UART_ReceivePassword(check);
	
	for (u8 i=0;i<6;i++)
	{
		if (check[i]!=password[i])
		{
			answer=Invalid;
			break;
		}
	} 
	return answer;
}

static void Password_change(void)
{
	LCD_Clear();
	LCD_WriteString(" Enter Password");
	if (check_password()==Valid)
	{
		LCD_Clear();
		LCD_WriteString("  New Password");
		u8 new[6];
		UART_ReceivePassword(new);
		
		for (u8 i=0;i<6;i++)
		{
			password[i]=new[i];
		}
		
		LCD_Clear();
		LCD_WriteStringGoto(1,6,"Done");
		EEPROM_write(0,1);
		EEPROM_InterruptEnable();
		EEPROM_EE_RDY_SetCallBack(Send_password);
		_delay_ms(1000);
		LCD_Clear();
	}
	else
	{
		LCD_Clear();
		LCD_WriteString("Invalid Password");
		_delay_ms(1000);
		LCD_Clear();
	}
	
}

static void Welcome (void)
{
	
	LCD_WriteString("   Smart Home");
	_delay_ms(1000);
	again:
	LCD_Clear();
	LCD_WriteString(" Enter Password");
	
	if (check_password()==Valid)
	{
		LCD_Clear();
		LCD_WriteString("  Welcome Home");
		_delay_ms(1000);
		mode=1;
	}
	else
	{
		LCD_Clear();
		LCD_WriteString(" Wrong Password");
		_delay_ms(1000);
		LCD_Clear();
		goto again;
	}
}

static void Led_control (void)
{
	DIO_Pin_type led=PINC0;
	
	uart=UART_Receive();
	
	if (uart==0xBB)
	{
		mode=1;
	}
	else if (uart==0xFF)
	{
		for (u8 i=0; i<8;i++)
		{
			DIO_WritePin(led+i,HIGH);
		}
	}
	else if (uart==0xEE)
	{
		for (u8 i=0; i<8;i++)
		{
			DIO_WritePin(led+i,LOW);
		}
	}
	else if (uart>=0 && uart<=7)
	{
		DIO_TogglePin(led+uart);
	}
}

static void Write_String (void)
{
	for (u8 i=1 ; i<32 ;i++)
	{
		str[i]=0;
	}
	UART_ReceiveString2(str);
	if (str[0]==0xBB)
	{
		c=0;
		mode=1;
		Lcd_control_mode=0;
	}
	else if (str[0]==0xF5)
	{
		LCD_Clear();
	}
	else
	{
		LCD_WriteString(str);
	}
}

static void Write_char (void)
{
	if (uart>=20 && uart<=127)
	{
		
		if (c==32)
		{
			LCD_Clear();
			c=0;
		}
		
		LCD_WriteChar(uart);
		c++;
		if (c==16)
		{
			LCD_GoTo(1,0);
		}
		
	}
}

static void Lcd_control (void)
{
	if (Lcd_control_mode==0)
	{
		LCD_WriteStringGoto(0,6,"Lcd");
		Lcd_control_mode=1;
	}
	else if (Lcd_control_mode==0xF4)
	{
		Write_String();
	}
	else
	{
		uart=UART_Receive();
		
		if (uart==0xBB)
		{
			c=0;
			mode=1;
			Lcd_control_mode=0;
		}
		else if (uart== 0xF4)
		{
			Lcd_control_mode=uart;
			LCD_Clear();
		}
		else if (uart==0xF5)
		{
			LCD_Clear();
			Lcd_control_mode=1;
			c=0;
		}
		else
		{
			if (Lcd_control_mode==1)
			{
				LCD_Clear();
				Lcd_control_mode=2;
			}
			Write_char();
		}
	}
	
}

static void Rgb_2button_mode (void)
{
	if (uart==0x14)
	{
		if (c>0)
		{
			c--;
		}
	}
	else if (uart==0x15)
	{
		if (c<(num_of_colors-1))
		{
			c++;
		}
	} 
	LCD_GoTo(1,0);
	LCD_WriteNumber(RGB_colors[c].Red);
	LCD_WriteString("  ");
	LCD_GoTo(1,5);
	LCD_WriteNumber(RGB_colors[c].Green);
	LCD_WriteString("  ");
	LCD_GoTo(1,10);
	LCD_WriteNumber(RGB_colors[c].Blue);
	LCD_WriteString("  ");
	RGB_color(RGB_colors[c]);
}

static void Rgb_input_mode (void)
{
	u8 rgb[3];
	
	for (u8 i=0 ; i<3 ; i++)
	{
		rgb[i]=UART_Receive();
		
		if (rgb[i]==0xBB)
		{
			Rgb_mode=0;
			break;
		}
	}
	RGB_color_array(rgb);
	LCD_GoTo(1,0);
	LCD_WriteNumber(rgb[0]);
	LCD_WriteString("  ");
	LCD_GoTo(1,5);
	LCD_WriteNumber(rgb[1]);
	LCD_WriteString("  ");
	LCD_GoTo(1,10);
	LCD_WriteNumber(rgb[2]);
	LCD_WriteString("  ");
}

static void RGB_control (void)
{
	if (Rgb_mode==0xB1)
	{
		Rgb_input_mode();
	}
	else
	{
		uart=UART_Receive();
		
		if (uart==0xBB)
		{
			c=0;
			mode=1;
			Rgb_mode=0;
		}
		else if (uart==0xB1)
		{
			Rgb_mode=uart;
		}
		else
		{
			Rgb_2button_mode();
		}
	}
	
}

void Smart_Home_Runnable (void)
{
	if (mode==0)
	{
		Welcome();
	}
	
	if (mode==1)
	{
		LCD_Clear();
		LCD_WriteString("  Welcome Home");
		
		uart=UART_Receive();
		
		if (uart==0x97 || uart==0x98 || uart==0x99 || uart==0xCC)
		{
			mode=uart;
			LCD_Clear();
		}
	}
	
	if (mode==0x97)
	{
		Lcd_control();
	}
	else if (mode==0x98)
	{
		LCD_WriteStringGoto(0,6,"Led");
		Led_control();
	}
	else if (mode==0x99)
	{
		LCD_WriteStringGoto(0,6,"RGB");
		RGB_control();
		
	}
	else if (mode==0xCC)
	{
		Password_change();
		mode=1;
	}
}





static void Send_password (void)
{
	EEPROM_write_array(1,password,6);
}