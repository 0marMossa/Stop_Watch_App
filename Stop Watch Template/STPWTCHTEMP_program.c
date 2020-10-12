/***********************************************************************************************/
/*********	Name	:	Omar Mossa			****************************************************/
/*********  Date	:	9/10/2020			****************************************************/
/*********	SWC		:	Stop Watch Template	****************************************************/
/*********	Version	:	V1.0				****************************************************/
/***********************************************************************************************/


#include "STD_TYPES.h"


#include "STPWTCHTEMP_interface.h"

#include "LCD_interface.h"
#include "TIMER_interface.h"


/*** POSITION OF DIGITS ***/
#define STPWTCHTEMP_SEC_ONES_POS	15
#define STPWTCHTEMP_SEC_TENS_POS	114
#define STPWTCHTEMP_MIN_ONES_POS	10
#define STPWTCHTEMP_MIN_TENS_POS	9
#define STPWTCHTEMP_HOUR_ONES_POS	5
#define STPWTCHTEMP_HOUR_TENS_POS	4
/**************************/


/*** POSITION OF DIGITS ***/
#define STPWTCHTEMP_SEC_ONES_INDEX	5
#define STPWTCHTEMP_SEC_TENS_INDEX	4
#define STPWTCHTEMP_MIN_ONES_INDEX	3
#define STPWTCHTEMP_MIN_TENS_INDEX	2
#define STPWTCHTEMP_HOUR_ONES_INDEX	1
#define STPWTCHTEMP_HOUR_TENS_INDEX	0
/**************************/



static u8 STPWTCHTEMP_Au8DigitsPositions[6] = { STPWTCHTEMP_HOUR_TENS_POS , STPWTCHTEMP_HOUR_ONES_POS , STPWTCHTEMP_MIN_TENS_POS , STPWTCHTEMP_MIN_ONES_POS , STPWTCHTEMP_SEC_TENS_POS , STPWTCHTEMP_SEC_ONES_POS } ; 
static u8 STPWTCHTEMP_u8CursorPosition = STPWTCHTEMP_Au8DigitsPositions[0] ;
static u8 STPWTCHTEMP_Au8DisplayedDigits[6] = { 0 , 0 , 0 , 0 , 0 , 0 } ;

void STPWTCHTEMP_voidInit(void)
{
	LCD_voidInit(void) ;
	LCD_u8SendString(" Hours MIN  SEC") ;

	LCD_u8GoToXY(LCD_LINE2 , 4 ) ;
	LCD_u8SendData('0') ;//4
	LCD_u8SendData('0') ;//5
	LCD_u8SendData(' ') ;//6
	LCD_u8SendData(':') ;//7
	LCD_u8SendData(' ') ;//8
	LCD_u8SendData('0') ;//9
	LCD_u8SendData('0') ;//10
	LCD_u8SendData(' ') ;//11
	LCD_u8SendData(':') ;//12
	LCD_u8SendData(' ') ;//13
	LCD_u8SendData('0') ;//14
	LCD_u8SendData('0') ;//15
	LCD_u8SendData(' ') ;//16

	LCD_u8GoToXY( LCD_LINE2  , STPWTCHTEMP_Au8DigitsPositions[ STPWTCHTEMP_HOUR_TENS_INDEX ] ) ; // set the cursor to the Hours digit
}


u8 STPWTCHTEMP_u8GetAndSetAction( u8 Copy_u8WantedAction ) 
{
	u8 Local_u8ErrorStatus = OK ;
	if ( Copy_u8WantedAction > '0' && Copy_u8WantedAction < '10' )
	{
		switch(STPWTCHTEMP_u8CursorPosition)
		{
			case STPWTCHTEMP_HOUR_TENS_POS :
			STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_HOUR_TENS_INDEX ] = Copy_u8WantedAction - '0' ;
			LCD_u8SendData(Copy_u8WantedAction);
			break ;

			case STPWTCHTEMP_HOUR_ONES_POS :
			STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_HOUR_ONES_INDEX ] = Copy_u8WantedAction - '0' ;
			LCD_u8SendData(Copy_u8WantedAction);
			break ;

			case STPWTCHTEMP_MIN_TENS_POS :
			STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_MIN_TENS_INDEX ] = Copy_u8WantedAction - '0' ;
			LCD_u8SendData(Copy_u8WantedAction);
			break ;

			case STPWTCHTEMP_MIN_ONES_POS :
			STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_MIN_ONES_INDEX ] = Copy_u8WantedAction - '0' ;
			LCD_u8SendData(Copy_u8WantedAction);
			break ;

			case STPWTCHTEMP_SEC_TENS_POS :
			STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_SEC_TENS_INDEX ] = Copy_u8WantedAction - '0' ;
			LCD_u8SendData(Copy_u8WantedAction);
			break ;

			case STPWTCHTEMP_SEC_ONES_POS :
			STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_SEC_ONES_INDEX ] = Copy_u8WantedAction - '0' ;
			LCD_u8SendData(Copy_u8WantedAction);
			break ;
		}
	}
	else if( Copy_u8WantedAction == '<' )
	{
		LCD_voidShiftCursorLeft();
	}
	else if ( Copy_u8WantedAction == '>' )
	{
		LCD_voidShiftCursorRight();
	}
	else if ( Copy_u8WantedAction == '=' )
	{
		STPWTCHTEMP_voidStartCountingDown();
	}
	else
	{
		Local_u8ErrorStatus = NOK ;
	}
	return Local_u8ErrorStatus ;
}


void STPWTCHTEMP_voidStartCountingDown(void)
{
	static u32 Local_u32TotalTime = 0 ;
	u8 Local_u8TotalSeconds = STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_SEC_ONES_INDEX ] + ( STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_SEC_TENS_INDEX ] * 10 ) ;
	u8 Local_u8TotalMinutes = STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_MIN_ONES_INDEX ] + ( STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_MIN_TENS_INDEX ] * 10 ) ;
	u8 Local_u8TotalHours = STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_HOUR_ONES_INDEX ] + ( STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_HOUR_TENS_INDEX ] * 10 ) ;
	Local_u32TotalTimeInSec =( Local_u8TotalHours * 3600 ) + ( Local_u8TotalMinutes  * 60 ) +  Local_u8TotalSeconds ;

	TIMER0_u8SetPeriodicEvent( 1000000 , void (*Copy_pfPeriodicISR)(void)) ;
}