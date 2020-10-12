/***********************************************************************************************/
/*********	Name	:	Omar Mossa			****************************************************/
/*********  Date	:	9/10/2020			****************************************************/
/*********	SWC		:	Stop Watch Template	****************************************************/
/*********	Version	:	V1.0				****************************************************/
/***********************************************************************************************/

#ifndef STPWTCHTEMP_interface
#define STPWTCHTEMP_interface



void STPWTCHTEMP_voidInit(void);
u8 STPWTCHTEMP_u8GetAndSetAction( u8 Copy_u8WantedAction ) ;
u8 STPWTCHTEMP_u8GetDigit( u8 Copy_u8DigitValue , u8 Copy_u8DigitPosition ) ;
void STPWTCHTEMP_u8CheckTimerNotification(void);



#endif