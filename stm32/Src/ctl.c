
/******************************************************************************
*  File           :  ctl.c
*  Version        :  1
*  Last Changes   :  2019-05-12 12:47:33
******************************************************************************/

/******************************************************************************
*  Include Files
******************************************************************************/

#include "main.h"
#include "ctl.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"
#include "ser1.h"
#include "ser2.h"
#include "ser3.h"
#include "typ.h"
#include "ste1.h"
#include "tim4.h"
#include <string.h>

/******************************************************************************
*  Local Constants
******************************************************************************/

// Protocol Positions
#define CMD						0
#define DIRECTION				1
#define PWM						2
#define DATA					1
#define DELAY					4

// Commands
#define CMD_INIT 					0x11
#define CMD_TEST_MOSFETS 			0x12
#define CMD_HALL 					0x13
#define CMD_SENSORLESS				0x14
#define CMD_MANUAL_COMMUTATION		0x15
#define CMD_SET_HALL_INFORMATION	0x16
#define CMD_STEUERUNG				0x17
#define CMD_REGELUNG				0x18
#define CMD_GET_STATE_REQUEST		0x19
#define CMD_GET_STATE_RESPONSE		0x20
#define SET_MOSFETS					0x21
#define SET_HALL_DETECTION			0x22
#define GET_HALL_INFO				0x23
#define SET_DIR_PWM_DELAY			0x24
#define SET_DIR_PWM					0x25
#define SET_RPS_INFO				0x26
#define GET_RPS_INFO				0x27

#define TEST_MOSFETS_1H     1
#define TEST_MOSFETS_1L     2
#define TEST_MOSFETS_2H     3
#define TEST_MOSFETS_2L     4
#define TEST_MOSFETS_3H     5
#define TEST_MOSFETS_3L     6

/******************************************************************************
*  Local Type Definitions
******************************************************************************/

/******************************************************************************
*  Local Variables
******************************************************************************/

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart3_tx;


enum {INIT = 0, NORMAL = 1, TIMEOUT = 2} ctl_State = TIMEOUT;

/******************************************************************************
*  Global Variables
******************************************************************************/

TUINT8 CTL_Temp = 0;

TUINT8 au8Temp[256];
TUINT8 u8Temp;

tyun_AnyData unAnyData;

TUINT32 CTL_u32Timeout_Counter;

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

void CTL_Ini (void)
{

	///////////////////////////////////////////////
	// Init BLDC Motors
	///////////////////////////////////////////////

	// Wait for BLCD Controller startup. Then send messages.
	HAL_Delay(1000);

	// Application Mode
	au8Temp[CMD] = CMD_HALL;
	SER2_Send_Protocol(au8Temp, 1);
	SER1_Send_Protocol(au8Temp, 1);
	HAL_UART_Transmit_DMA(&huart1, SER1_au8SndBuf, SER1_TransmitFrameLength);
	HAL_UART_Transmit_DMA(&huart2, SER2_au8SndBuf, SER2_TransmitFrameLength);
	HAL_Delay(100);

	// Hall Information
	au8Temp[CMD] = CMD_SET_HALL_INFORMATION;
	au8Temp[DATA + 0] = 6;
	au8Temp[DATA + 1] = 2;
	au8Temp[DATA + 2] = 1;
	au8Temp[DATA + 3] = 4;
	au8Temp[DATA + 4] = 5;
	au8Temp[DATA + 5] = 3;
	SER2_Send_Protocol(au8Temp, 7);
	SER1_Send_Protocol(au8Temp, 7);
	HAL_UART_Transmit_DMA(&huart1, SER1_au8SndBuf, SER1_TransmitFrameLength);
	HAL_UART_Transmit_DMA(&huart2, SER2_au8SndBuf, SER2_TransmitFrameLength);
	HAL_Delay(100);

	// Operation Mode
	au8Temp[CMD] = CMD_STEUERUNG;
	SER2_Send_Protocol(au8Temp, 1);
	SER1_Send_Protocol(au8Temp, 1);
	HAL_UART_Transmit_DMA(&huart1, SER1_au8SndBuf, SER1_TransmitFrameLength);
	HAL_UART_Transmit_DMA(&huart2, SER2_au8SndBuf, SER2_TransmitFrameLength);
	HAL_Delay(100);

	// Get State
	au8Temp[CMD] = CMD_GET_STATE_REQUEST;
	SER2_Send_Protocol(au8Temp, 1);
	SER1_Send_Protocol(au8Temp, 1);
	HAL_UART_Transmit_DMA(&huart1, SER1_au8SndBuf, SER1_TransmitFrameLength);
	HAL_UART_Transmit_DMA(&huart2, SER2_au8SndBuf, SER2_TransmitFrameLength);
	HAL_Delay(100);




}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

void CTL_Main (void)
{

  /*
  switch (ctl_State)
  {
  	  case NORMAL:
  		  if (CTL_u32Timeout_Counter > 1000)
  		  {
				// Linker Motor
				au8Temp[CMD] = SET_DIR_PWM;
				au8Temp[DIRECTION] = 0;
				unAnyData.u16Data = 0;
				au8Temp[PWM + 0] = unAnyData.au8Data[0];
				au8Temp[PWM + 1] = unAnyData.au8Data[1];
				SER2_Send_Protocol(au8Temp, 4);
				HAL_UART_Transmit_DMA(&huart2, SER2_au8SndBuf, SER2_TransmitFrameLength);
				HAL_Delay(10);



				// Rechter Motor
				au8Temp[CMD] = SET_DIR_PWM;
				au8Temp[DIRECTION] = 0;
				unAnyData.u16Data = 0;
				au8Temp[PWM + 0] = unAnyData.au8Data[0];
				au8Temp[PWM + 1] = unAnyData.au8Data[1];
				SER1_Send_Protocol(au8Temp, 4);
				HAL_UART_Transmit_DMA(&huart1, SER1_au8SndBuf, SER1_TransmitFrameLength);
				HAL_Delay(10);

  			  ctl_State = TIMEOUT;
  		  }
  		  break;

  	  case TIMEOUT:
  		  	  	  	  break;

  	  default: break;
  }*/



  if (!SER3_u8Valid_Data())
  {
	SER3_Protocol_Handling(100);
  }
  else
  {


	tystSteering stSteering;
	TUINT16 u16Thro;
	TUINT16 u16Elev;
	TUINT8 u8FailSafe_LED_Blinking;
	TUINT8 u8FailSafe_LED_Status;

	// Thro
	unAnyData.au8Data[0] = SER3_au8RecBuf[0];
	unAnyData.au8Data[1] = SER3_au8RecBuf[1];
	u16Thro = unAnyData.u16Data;

	// Elev
	unAnyData.au8Data[0] = SER3_au8RecBuf[2];
	unAnyData.au8Data[1] = SER3_au8RecBuf[3];
	u16Elev = unAnyData.u16Data;

	// FailSafe
	u8FailSafe_LED_Blinking = SER3_au8RecBuf[16];
	u8FailSafe_LED_Status = SER3_au8RecBuf[17];

	memset((char*)SER3_au8RecBuf, 0, sizeof(SER3_au8RecBuf));


	if (((u16Thro >= 1070) && (u16Thro <= 1920)) && ((u16Elev >= 1070) && (u16Elev <= 1920)) && ((u8FailSafe_LED_Blinking == 1) || (u8FailSafe_LED_Status == 1)))
	{
		ctl_State = NORMAL;
		CTL_u32Timeout_Counter = 0;



		stSteering = STE1_Calculation (u16Thro, u16Elev);

		// Linker Motor
		au8Temp[CMD] = SET_DIR_PWM;
		au8Temp[DIRECTION] = stSteering.u8Motor_Left_Dir;
		unAnyData.u16Data = stSteering.u16Motor_Left_Speed * 5;
		au8Temp[PWM + 0] = unAnyData.au8Data[0];
		au8Temp[PWM + 1] = unAnyData.au8Data[1];
		SER2_Send_Protocol(au8Temp, 4);
		HAL_UART_Transmit_DMA(&huart2, SER2_au8SndBuf, SER2_TransmitFrameLength);
		HAL_Delay(10);



		// Rechter Motor
		au8Temp[CMD] = SET_DIR_PWM;
		au8Temp[DIRECTION] = stSteering.u8Motor_Right_Dir ? 0 : 1;
		unAnyData.u16Data = stSteering.u16Motor_Right_Speed * 5;
		au8Temp[PWM + 0] = unAnyData.au8Data[0];
		au8Temp[PWM + 1] = unAnyData.au8Data[1];
		SER1_Send_Protocol(au8Temp, 4);
		HAL_UART_Transmit_DMA(&huart1, SER1_au8SndBuf, SER1_TransmitFrameLength);
		HAL_Delay(10);
	}
	else
	{

		// Linker Motor
		au8Temp[CMD] = SET_DIR_PWM;
		au8Temp[DIRECTION] = 1;
		unAnyData.u16Data = 0;
		au8Temp[PWM + 0] = unAnyData.au8Data[0];
		au8Temp[PWM + 1] = unAnyData.au8Data[1];
		SER2_Send_Protocol(au8Temp, 4);
		HAL_UART_Transmit_DMA(&huart2, SER2_au8SndBuf, SER2_TransmitFrameLength);
		HAL_Delay(10);



		// Rechter Motor
		au8Temp[CMD] = SET_DIR_PWM;
		au8Temp[DIRECTION] = 1 ? 0 : 1;
		unAnyData.u16Data = 0;
		au8Temp[PWM + 0] = unAnyData.au8Data[0];
		au8Temp[PWM + 1] = unAnyData.au8Data[1];
		SER1_Send_Protocol(au8Temp, 4);
		HAL_UART_Transmit_DMA(&huart1, SER1_au8SndBuf, SER1_TransmitFrameLength);
		HAL_Delay(10);
	}



  }


}

/******************************************************************************
*  Changes        :  
*                    
*                    
*                    
******************************************************************************/

/******************************************************************************
*  END OF FILE    :  ctl.h
******************************************************************************/
