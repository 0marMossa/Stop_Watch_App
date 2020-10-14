#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "DIO_interface.h"
#include "GIE_interface.h"
#include "TIMER_interface.h"


#include "KPD_interface.h"
#include "LCD_interface.h"

#include "STPWTCHTEMP_interface.h"
#include "STPWTCHTIMER_interface.h"

/*** Options for u8 STPWTCHTEMP_u8StopWatchStatus ***/
#define COUNTING 			0
#define NOT_COUNTING		1
/****************************************************/

extern u8 STPWTCHTEMP_u8StopWatchStatus ;
int main(void)
{
	DIO_u8PortsInit();
	LCD_voidInit();
	GIE_voidEnableGeneralInt() ;
	STPWTCHTEMP_voidInit();
	u8 Local_u8PressedKey = KPD_KEY_NOT_PRESSED ;


	while(1)
	{
		while(1)
		{
			/*** Setup Mode ***/
			Local_u8PressedKey = KPD_KEY_NOT_PRESSED ;
			while(Local_u8PressedKey == KPD_KEY_NOT_PRESSED)
			{
				KPD_u8GetKeyValue(&Local_u8PressedKey); //get pressed key
			}
			if(Local_u8PressedKey == '=')
			{
				/*** start the stop watch and Go to Operation Mode ***/
				STPWTCHTEMP_voidStartCountingDown(); // start the stop watch
				break ; // exit setup mode
			}
			else
			{
				STPWTCHTEMP_u8GetAndSetAction(Local_u8PressedKey); // do the suitable action and display it on LCD
			}
		}

		while(1)
		{
			/*** Operation Mode ***/
			if(STPWTCHTEMP_u8StopWatchStatus == NOT_COUNTING)
			{
				/*** Go to Setup Mode ***/
				break ;
			}
			if(STPWTCHTEMP_u8StopWatchStatus == COUNTING)
			{
				STPWTCHTEMP_u8CheckAndUpdateStopWatch();
			}
		}

	}
	return 0 ;
}
