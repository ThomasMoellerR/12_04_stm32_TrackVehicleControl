
/******************************************************************************
*  File           :  ste1.h
*  Version        :  1
*  Last Changes   :  2019-05-11 21:41:16
******************************************************************************/

#ifndef __STE1_H
#define __STE1_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*  Include Files
******************************************************************************/

#include "typ.h"

/******************************************************************************
*  Global Constants
******************************************************************************/

/******************************************************************************
*  Global Type Definitions
******************************************************************************/

typedef struct
{
	TUINT16 u16Motor_Left_Speed;
	TUINT8 u8Motor_Left_Dir;
	TUINT16 u16Motor_Right_Speed;
	TUINT8 u8Motor_Right_Dir;
}tystSteering;

/******************************************************************************
*  Global Variables
******************************************************************************/

extern TUINT8 STE1_Temp;

/******************************************************************************
*  Global Function Prototypes
******************************************************************************/

extern void STE1_Ini (void);
extern tystSteering STE1_Calculation (TUINT16 u16Thro, TUINT16 u16Elev);

/******************************************************************************
*  Changes        :  
*                    
*                    
*                    
******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __STE1_H */

/******************************************************************************
*  END OF FILE    :  ste1.h
******************************************************************************/