/***********************************************************************************************/
/*********	Name	:	Omar Mossa			****************************************************/
/*********  Date	:	12/10/2020			****************************************************/
/*********	SWC		:	Stop Watch Timer	****************************************************/
/*********	Version	:	V1.0				****************************************************/
/***********************************************************************************************/


#include "STD_TYPES.h"


#include "STPWTCHTIMER_interface.h"
#include "TIMER_interface.h"


/*** INDEX OF DISPLAED DIGITS ***/
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



u8 STPWTCHTEMP_u8StopWatchStatus = NOT_COUNTING ;
u8 STPWTCHTEMP_u8TimeUpdateNotification = NO_UPDATE_NEEDED ;
static u32 STPWTCHTEMP_u32TotalTimeInSec = 0 ;
extern u8 STPWTCHTEMP_Au8DisplayedDigits[6] ;

void STPWTCHTEMP_voidStartCountingDown(void)
{
	STPWTCHTEMP_u8StopWatchStatus = COUNTING ;
	u8 Local_u8TotalSeconds = STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_SEC_ONES_INDEX ] + ( STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_SEC_TENS_INDEX ] * 10 ) ;
	u8 Local_u8TotalMinutes = STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_MIN_ONES_INDEX ] + ( STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_MIN_TENS_INDEX ] * 10 ) ;
	u8 Local_u8TotalHours = STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_HOUR_ONES_INDEX ] + ( STPWTCHTEMP_Au8DisplayedDigits[ STPWTCHTEMP_HOUR_TENS_INDEX ] * 10 ) ;
	STPWTCHTEMP_u32TotalTimeInSec =( Local_u8TotalHours * 3600 ) + ( Local_u8TotalMinutes  * 60 ) +  Local_u8TotalSeconds ;

	TIMER0_u8SetPeriodicEvent( 1000000UL , &STPWTCHTEMP_voidSetRemainingTime ) ;
}

void STPWTCHTEMP_voidSetRemainingTime( void )
{
	STPWTCHTEMP_u32TotalTimeInSec--; // One second has passed
	STPWTCHTEMP_u8TimeUpdateNotification = UPDATE_NEEDED ; // Need to update time on LCD 
}



