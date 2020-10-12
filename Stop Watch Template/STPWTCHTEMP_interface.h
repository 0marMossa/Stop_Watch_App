/***********************************************************************************************/
/*********	Name	:	Omar Mossa			****************************************************/
/*********  Date	:	9/10/2020			****************************************************/
/*********	SWC		:	Stop Watch Template	****************************************************/
/*********	Version	:	V1.0				****************************************************/
/***********************************************************************************************/

#ifndef STPWTCHTEMP_interface
#define STPWTCHTEMP_interface



void STPWTCHTEMP_voidInit(void);

u8 STPWTCHTEMP_u8GetRemainingTime( u32 Copy_u32RemainingTime );
u8 STPWTCHTEMP_u8GetDigit( u8 Copy_u8DigitValue , u8 Copy_u8DigitPosition ) ;

u8 STPWTCHTEMP_u8SetRemainingTime( u32 Copy_u32RemainingTime );

void STPWTCHTEMP_voidStartCountingDown(void);


#endif