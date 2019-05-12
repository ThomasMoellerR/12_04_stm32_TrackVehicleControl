
/******************************************************************************
*  File           :  ste1.c
*  Version        :  1
*  Last Changes   :  2019-05-11 21:41:16
******************************************************************************/

/******************************************************************************
*  Include Files
******************************************************************************/

#include "typ.h"
#include "ste1.h"
#include <stdlib.h>

/******************************************************************************
*  Local Constants
******************************************************************************/

// Elevator and Aile
//
//       1916
//  1919      1080
//       1079

#define FORWARD	1916
#define BACKWARD 1079
#define LEFT 1919
#define RIGHT 1080
#define VERTICAL_MIDDLE_POSTITION	((FORWARD + BACKWARD) / 2)
#define HORIZONTAL_MIDDLE_POSTITION	((LEFT + RIGHT) / 2)


/******************************************************************************
*  Local Type Definitions
******************************************************************************/

/******************************************************************************
*  Local Variables
******************************************************************************/

/******************************************************************************
*  Global Variables
******************************************************************************/

TUINT8 STE1_Temp = 0;

/******************************************************************************
*  Local Function Prototypes
******************************************************************************/

/******************************************************************************
*  Local Functions
******************************************************************************/

/******************************************************************************
*  Global Functions
******************************************************************************/

/******************************************************************************
*  Function Name  :  
*  Description    :  
*  Parameter(s)   :  
*  Return Value   :  
******************************************************************************/

void STE1_Ini (void)
{

}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

tystSteering STE1_Calculation (TUINT16 u16Thro, TUINT16 u16Elev)
{
  ///////////////////////////////////////////////////////////////////////////////
  //
  // Calculate curves
  //
  ///////////////////////////////////////////////////////////////////////////////

  int Throttle;
  Throttle = (int) ((float)0.954654 * (float)u16Thro - (float)1430.07);
  if (Throttle > 400) Throttle = 400;
  if (Throttle < -400) Throttle = -400;

  int Elevator;
  Elevator = (int) ((float)0.954654 * (float)u16Elev - (float)1430.07);
  if (Elevator > 400) Elevator = 400;
  if (Elevator < -400) Elevator = -400;

  //TUINT8 au8Temp[100];
  //sprintf((char*)au8Temp, "%d, %d\n", Throttle, Elevator);
  //SER2_Send_String(au8Temp, strlen(au8Temp));


  int Left_Track_Speed, Left_Track_Dir, Right_Track_Speed, Right_Track_Dir;

  if (Throttle >= 0)
  {
	Left_Track_Speed = abs(Throttle);
	Left_Track_Dir = 1;
  }
  else
  {
	Left_Track_Speed = abs(Throttle);
	Left_Track_Dir = 0;
  }

  if (Elevator >= 0)
  {
	Right_Track_Speed = abs(Elevator);
	Right_Track_Dir = 1;
  }
  else
  {
	Right_Track_Speed = abs(Elevator);
	Right_Track_Dir = 0;
  }


  //TUINT8 au8Temp[100];
  //sprintf((char*)au8Temp, "%d, %d, %d, %d\n", Left_Track_Speed, Left_Track_Dir,  Right_Track_Speed, Right_Track_Dir);
  //SER2_Send_String(au8Temp, strlen(au8Temp));



  tystSteering stSteering;



  stSteering.u8Motor_Left_Dir = Left_Track_Dir;
  stSteering.u16Motor_Left_Speed = Left_Track_Speed;
  stSteering.u8Motor_Right_Dir = Right_Track_Dir;
  stSteering.u16Motor_Right_Speed = Right_Track_Speed;






  return stSteering;

}

/******************************************************************************
*  Changes        :  
*                    
*                    
*                    
******************************************************************************/

/******************************************************************************
*  END OF FILE    :  ste1.h
******************************************************************************/
