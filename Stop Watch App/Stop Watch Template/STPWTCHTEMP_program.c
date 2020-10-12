/***********************************************************************************************/
/*********	Name	:	Omar Mossa			****************************************************/
/*********  Date	:	9/10/2020			****************************************************/
/*********	SWC		:	Stop Watch Template	****************************************************/
/*********	Version	:	V1.0				****************************************************/
/***********************************************************************************************/


#include "STD_TYPES.h"


#include "STPWTCHTEMP_interface.h"
#include "STPWTCHTEMP_private.h"

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

/*** Options for STPWTCHTEMP_u8TimeUpdateNotification ***/
#define NO_UPDATE_NEEDED 	0
#define UPDATE_NEEDED		1
/********************************************************/

/*** Options for u8 STPWTCHTEMP_u8StopWatchStatus ***/
#define COUNTING 			0
#define NOT_COUNTING		1
/****************************************************/




static u8 STPWTCHTEMP_Au8DigitsPositions[6] = { STPWTCHTEMP_HOUR_TENS_POS , STPWTCHTEMP_HOUR_ONES_POS , STPWTCHTEMP_MIN_TENS_POS , STPWTCHTEMP_MIN_ONES_POS , STPWTCHTEMP_SEC_TENS_POS , STPWTCHTEMP_SEC_ONES_POS } ; 
static u8 STPWTCHTEMP_u8CursorPosition = STPWTCHTEMP_Au8DigitsPositions[0] ;
u8 STPWTCHTEMP_Au8DisplayedDigits[6] = { 0 , 0 , 0 , 0 , 0 , 0 } ;
u32 STPWTCHTEMP_u32TotalTimeInSec = 0 ;
extern u8 STPWTCHTEMP_u8TimeUpdateNotification ;
extern u8 STPWTCHTEMP_u8StopWatchStatus ;

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


void STPWTCHTEMP_u8CheckTimerNotification(void)
{
	if ( STPWTCHTEMP_u8TimeUpdateNotification == UPDATE_NEEDED )
	{
		/*** Update the display with current remaining time ***/ 
		if ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_ONES_INDEX] != 0 )
		{
			/** Updating Seconds ones **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_ONES_INDEX]-- ; //update the saved value for this digit
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_SEC_ONES_POS] ) ; // go to SEC ones digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_ONES_INDEX] ) ;	// update it

			STPWTCHTEMP_u8TimeUpdateNotification == NO_UPDATE_NEEDED ; // update the notification flag
		}
		else if ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_TENS_INDEX] != 0 )
		{
			/** Updating Seconds ones **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_ONES_INDEX] = 9 ; //reset Secondes ones value 
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_SEC_ONES_POS] ) ; // go to SEC ones digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_ONES_INDEX] ) ;	// update it
	
			/** Updating Seconds Tens **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_TENS_INDEX]-- ; //update the saved value for this digit
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_SEC_TENS_POS] ) ; // go to SEC tens digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_TENS_INDEX] ) ;	// update it 

			STPWTCHTEMP_u8TimeUpdateNotification == NO_UPDATE_NEEDED ; // update the notification flag
		}
		else if ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_MIN_ONES_INDEX] != 0 )
		{
			/** Updating Seconds ones **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_ONES_INDEX] = 9 ; //reset Secondes ones value 
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_SEC_ONES_POS] ) ; // go to SEC ones digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_ONES_INDEX] ) ;	// update it

			/** Updating Seconds Tens **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_TENS_INDEX] = 5 ; //update the saved value for this digit
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_SEC_TENS_POS] ) ; // go to SEC tens digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_TENS_INDEX] ) ;	// update it 

			/** Updating Minutes ones **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_MIN_ONES_INDEX]-- ; //update the saved value for this digit
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_MIN_ONES_POS] ) ; // go to MIN ones digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_MIN_ONES_INDEX] ) ;	// update it 

			STPWTCHTEMP_u8TimeUpdateNotification == NO_UPDATE_NEEDED ; // update the notification flag
		}
		else if ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_MIN_TENS_INDEX] != 0 )
		{
			/** Updating Seconds ones **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_ONES_INDEX] = 9 ; //reset Secondes ones value 
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_SEC_ONES_POS] ) ; // go to SEC ones digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_ONES_INDEX] ) ;	// update it

			/** Updating Seconds Tens **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_TENS_INDEX] = 5 ; //update the saved value for this digit
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_SEC_TENS_POS] ) ; // go to SEC tens digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_TENS_INDEX] ) ;	// update it 

			/** Updating Minutes ones **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_MIN_ONES_INDEX] == 9; //update the saved value for this digit
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_MIN_ONES_POS] ) ; // go to MIN ones digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_MIN_ONES_INDEX] ) ;	// update it

			/** Updating Minutes tens **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_MIN_TENS_INDEX]--; //update the saved value for this digit
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_HOUR_TENS_POS] ) ; // go to hour tens digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_HOUR_TENS_INDEX] ) ;	// update it

			STPWTCHTEMP_u8TimeUpdateNotification == NO_UPDATE_NEEDED ; // update the notification flag
		}
		else if ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_HOUR_ONES_INDEX] != 0 )
		{
			/** Updating Seconds ones **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_ONES_INDEX] = 9 ; //reset Secondes ones value 
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_SEC_ONES_POS] ) ; // go to SEC ones digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_ONES_INDEX] ) ;	// update it

			/** Updating Seconds Tens **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_TENS_INDEX] = 5 ; //update the saved value for this digit
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_SEC_TENS_POS] ) ; // go to SEC tens digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_TENS_INDEX] ) ;	// update it 

			/** Updating Minutes ones **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_MIN_ONES_INDEX] == 9; //update the saved value for this digit
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_MIN_ONES_POS] ) ; // go to MIN ones digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_MIN_ONES_INDEX] ) ;	// update it

			/** Updating Minutes tens **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_MIN_TENS_INDEX] = 5; //update the saved value for this digit
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_HOUR_TENS_POS] ) ; // go to hour tens digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_HOUR_TENS_INDEX] ) ;	// update it

			/** Updating Hours ones **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_HOUR_ONES_INDEX]-- ; // update the saved value for this digit
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_HOUR_ONES_POS] ) ; // go to hour ones digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_HOUR_ONES_INDEX] ) ;	// update it

			STPWTCHTEMP_u8TimeUpdateNotification == NO_UPDATE_NEEDED ; // update the notification flag
		}
		else if ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_HOUR_TENS_INDEX] != 0 )
		{
			/** Updating Seconds ones **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_ONES_INDEX] = 9 ; //reset Secondes ones value 
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_SEC_ONES_POS] ) ; // go to SEC ones digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_ONES_INDEX] ) ;	// update it

			/** Updating Seconds Tens **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_TENS_INDEX] = 5 ; //update the saved value for this digit
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_SEC_TENS_POS] ) ; // go to SEC tens digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_SEC_TENS_INDEX] ) ;	// update it 

			/** Updating Minutes ones **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_MIN_ONES_INDEX] == 9; //update the saved value for this digit
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_MIN_ONES_POS] ) ; // go to MIN ones digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_MIN_ONES_INDEX] ) ;	// update it

			/** Updating Minutes tens **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_MIN_TENS_INDEX] = 5; //update the saved value for this digit
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_HOUR_TENS_POS] ) ; // go to hour tens digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_HOUR_TENS_INDEX] ) ;	// update it

			/** Updating Hours ones **/
			STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_HOUR_ONES_INDEX] = 9 ; // update the saved value for this digit
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_HOUR_ONES_POS] ) ; // go to hour ones digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_HOUR_ONES_INDEX] ) ;	// update it

			/** Update Hours Tens **/
			STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_HOUR_TENS_POS]-- ; // update the saved value for this digit
			LCD_u8GoToXY(LCD_LINE2 , STPWTCHTEMP_Au8DigitsPositions[STPWTCHTEMP_HOUR_TENS_POS] ) ; // go to MIN tens digit
			LCD_u8SendData ( STPWTCHTEMP_Au8DisplayedDigits[STPWTCHTEMP_HOUR_TENS_INDEX] ) ;	// update it

			STPWTCHTEMP_u8TimeUpdateNotification == NO_UPDATE_NEEDED ; // update the notification flag
		}
		else 
		{
			/** The Counter Reached ZERO **/ /** Finally XD **/
			TIMER0_voidStopTimer0(); // Stop the timer 
			STPWTCHTEMP_u8TimeUpdateNotification == NO_UPDATE_NEEDED ; // update the notification flag
			STPWTCHTEMP_u8StopWatchStatus = NOT_COUNTING ; // update stop watch status
		}	
	}
	else
	{
		/*** DO NOTHING ***/
	}
}