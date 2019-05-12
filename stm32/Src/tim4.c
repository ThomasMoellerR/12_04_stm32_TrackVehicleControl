
/******************************************************************************
*  File           :  tim4.c
*  Version        :  1
*  Last Changes   :  2019-05-12 12:52:46
******************************************************************************/

/******************************************************************************
*  Include Files
******************************************************************************/

#include "typ.h"
#include "tim4.h"
#include "main.h"
#include "ctl.h"

/******************************************************************************
*  Local Constants
******************************************************************************/

/******************************************************************************
*  Local Type Definitions
******************************************************************************/

/******************************************************************************
*  Local Variables
******************************************************************************/

TIM_HandleTypeDef htim4;

/******************************************************************************
*  Global Variables
******************************************************************************/

TUINT8 TIM4_Temp = 0;

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

void TIM4_Ini (void)
{
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

void TIM4_HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	CTL_u32Timeout_Counter++;
}

/******************************************************************************
*  Changes        :  
*                    
*                    
*                    
******************************************************************************/

/******************************************************************************
*  END OF FILE    :  tim4.h
******************************************************************************/