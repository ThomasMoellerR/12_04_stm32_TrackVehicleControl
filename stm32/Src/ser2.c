
/******************************************************************************
*  File           :  ser2.c
*  Version        :  1
*  Last Changes   :  2019-05-12 11:06:06
******************************************************************************/

/******************************************************************************
*  Include Files
******************************************************************************/

#include <string.h>
#include "typ.h"
#include "ser2.h"

/******************************************************************************
*  Local Constants
******************************************************************************/

/******************************************************************************
*  Local Type Definitions
******************************************************************************/

enum tyen_Operation
{
  PROOF_STX,
  STORE_DLC,
  STORE_DATA,
  PROOF_CHK,
  PROOF_ETX
};

/******************************************************************************
*  Local Variables
******************************************************************************/

enum tyen_Operation ser2_Operation = PROOF_STX;

TUINT8 ser2_au8SndBuf[SER2_SNDBUF_QUELEN + 1];
TUINT16 ser2_u16SndBuf_Head = 0;
TUINT16 ser2_u16SndBuf_Tail = 0;

TUINT8 ser2_au8RecBuf[SER2_RECBUF_QUELEN + 1];
TUINT16 ser2_u16RecBuf_Head = 0;
TUINT16 ser2_u16RecBuf_Tail = 0;

TUINT8 ser2_u8Valid_Data;

/******************************************************************************
*  Global Variables
******************************************************************************/

// Sending
TUINT8 SER2_u8SendPollingRunning; 				// Indicates the polling state if polling is used. For DMA transfer not relevant
TUINT8 SER2_au8SndBuf[SER2_SNDBUF_LEN]; 		// Frame with payload to send
TUINT8 SER2_TransmitFrameLength;				// For DMA transfer

// Receiving
TUINT8 SER2_au8RecBuf[SER2_RECBUF_LEN]; 		// Extracted payload of received frame
TUINT8 SER2_u8ReceivedPayloadLength; 					// Number of data in received payload

/******************************************************************************
*  Local Function Prototypes
******************************************************************************/

void ser2_Start_Sending (void);
TUINT8 ser2_XORChecksum(TUINT8* pu8Data, TUINT8 u8Len);

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

void SER2_Ini (void)
{
  SER2_u8SendPollingRunning = 0;
  ser2_Operation = PROOF_STX;
  ser2_u8Valid_Data = 0;
}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

TUINT8 ser2_XORChecksum(TUINT8* pu8Data, TUINT8 u8Len)
{
  TUINT8 u8XOR = 0;
  for(int i = 0; i < u8Len; i++) u8XOR ^= pu8Data[i];
  return u8XOR;
}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

void SER2_Protocol_Handling(TUINT8 u8Cycles)
{
  static TUINT8 u8Temp;
  static TUINT8 u8DLC;
  static TUINT8 u8Counter;
  static TUINT8 u8Chk;
  static TUINT8 u8i;
  int i;

  for (i = 0; i < u8Cycles; i++)
  {
    if ((!SER2_u8RecBuf_Empty()) && (!ser2_u8Valid_Data))
    {
      switch (ser2_Operation)
      {
        case PROOF_STX:
        {
          u8Temp = SER2_u8RecBuf_Get();

          if (u8Temp == SER2_STX) // Ist das Startzeichen korrekt?
          {
            ser2_Operation = STORE_DLC;
          }

          break;
        }

        case STORE_DLC:
        {
          u8DLC = SER2_u8RecBuf_Get(); // DLC speichern
          u8Counter = 0; // Wichtig, nicht vergessen, hat schon oft Sorgen bereitet
          ser2_Operation = STORE_DATA;

          break;
        }

        case STORE_DATA: // Daten speichern
        {
          if (u8Counter < u8DLC)
          {
            SER2_au8RecBuf[u8Counter] = SER2_u8RecBuf_Get();
            u8Counter++;
          }
          else
          {
      	    ser2_Operation = PROOF_CHK;
          }

          break;
        }

        case PROOF_CHK: // Checksumme berechnen und mit Checksumme von Protokoll vergleichen
        {
          u8Chk = 0;

          u8Chk ^= SER2_STX;
          u8Chk ^= u8DLC;

          for (u8i = 0; u8i < u8DLC; u8i++)
          {
            u8Chk ^=  SER2_au8RecBuf[u8i];
          }

          u8Temp = SER2_u8RecBuf_Get(); // Hole die Checksumme

          if (u8Temp == u8Chk) // �berpr�fe die Checksumme
          {
            ser2_Operation = PROOF_ETX;
          }
          else
          {
            ser2_Operation = PROOF_STX; // Versuch verwerfen
          }

          break;
        }

        case PROOF_ETX:
        {
          u8Temp = SER2_u8RecBuf_Get();

         if (u8Temp == SER2_ETX) // Ist das Ende Zeichen korrekt?
          {
            ser2_u8Valid_Data = 1; // Jetzt sind die Daten g�ltig ansonsten nicht
            SER2_u8ReceivedPayloadLength = u8DLC; // DLC einer globalen Variable �bergeben
            ser2_Operation = PROOF_STX; // Von vorne anfangen
          }
          else
          {
            ser2_Operation = PROOF_STX; // Von vorne anfangen
          }

          break;
        }

        default: break;
      }
    }
  }
}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

TUINT8 SER2_u8Valid_Data(void)
{
  TUINT8 u8Ret_Val;

  if (ser2_u8Valid_Data)
  {
    ser2_u8Valid_Data = 0;
    u8Ret_Val = 1;
  }
  else
  {
	u8Ret_Val = 0;
  }

  return u8Ret_Val;
}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

void SER2_Send_Protocol (TUINT8* pu8Data, TUINT8 u8Len)
{
  // STX, DLC, DATA
  SER2_au8SndBuf[0] = SER2_STX;
  SER2_au8SndBuf[1] = u8Len;
  memcpy((char*)&SER2_au8SndBuf[2], (char*)pu8Data, u8Len);

#ifdef XOR_CHECKSUM
  TUINT8 u8Chk;
  u8Chk = ser2_XORChecksum(SER2_au8SndBuf, u8Len + 2);
#endif


#ifdef SER2_SENDING_MODE_POLLING
  int i;
  SER2_SndBuf_Put(SER2_au8SndBuf[0]);
  SER2_SndBuf_Put(u8Len);
  for (i = 2; i < u8Len + 2; i++) SER2_SndBuf_Put (*(pu8Data + u8i));
  SER2_SndBuf_Put(u8Chk);
  SER2_SndBuf_Put (SER2_ETX);

  // Sendevorgang anstossen
  ser2_Start_Sending();
#endif


#ifdef SER2_SENDING_MODE_DMA
  SER2_au8SndBuf[u8Len + 2] = u8Chk;
  SER2_au8SndBuf[u8Len + 3] = SER2_ETX;

  SER2_TransmitFrameLength = u8Len + 4;
#endif
}


#ifdef SER2_SENDING_MODE_POLLING

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

void SER2_Send_String (TUINT8* pu8Data, TUINT8 u8Len)
{
  TUINT16 u8i;

  // DATA
  for (u8i = 0; u8i < u8Len; u8i++)
  {
	SER2_SndBuf_Put (*(pu8Data + u8i));
  }

  // Sendevorgang anstossen
  ser2_Start_Sending();
}

#endif


/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

#ifdef SER2_SENDING_MODE_POLLING

void ser2_Start_Sending (void)
{
  TUINT8 u8Temp;

  // Sendevorgang ansto�en, wenn gerade nichts gesendet wird
  if (!SER2_u8SendPollingRunning)
  {
	SER2_u8SendPollingRunning = 1;
	u8Temp = SER2_u8SndBuf_Get();
	//USART_SendData(USART3, u8Temp);
  }
}

#endif


/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

void SER2_SndBuf_Put (TUINT8 u8Data)
{
  ser2_au8SndBuf[ser2_u16SndBuf_Tail++] = u8Data;
  if (ser2_u16SndBuf_Tail > SER2_SNDBUF_QUELEN) ser2_u16SndBuf_Tail = 0;
}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

TUINT8 SER2_u8SndBuf_Get (void)
{
  TUINT8 u8Data = ser2_au8SndBuf[ser2_u16SndBuf_Head++];
  if (ser2_u16SndBuf_Head > SER2_SNDBUF_QUELEN) ser2_u16SndBuf_Head = 0;
  return u8Data;
}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

TUINT8 SER2_u8SndBuf_Empty (void)
{
  return ser2_u16SndBuf_Head == ser2_u16SndBuf_Tail;
}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

void SER2_RecBuf_Put (TUINT8 u8Data)
{
  ser2_au8RecBuf[ser2_u16RecBuf_Tail++] = u8Data;
  if (ser2_u16RecBuf_Tail > SER2_RECBUF_QUELEN) ser2_u16RecBuf_Tail = 0;
}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

TUINT8 SER2_u8RecBuf_Get (void)
{
  TUINT8 u8Data = ser2_au8RecBuf[ser2_u16RecBuf_Head++];
  if (ser2_u16RecBuf_Head > SER2_RECBUF_QUELEN) ser2_u16RecBuf_Head = 0;
  return u8Data;
}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

TUINT8 SER2_u8RecBuf_Empty (void)
{
  return ser2_u16RecBuf_Head == ser2_u16RecBuf_Tail;
}


/******************************************************************************
*  Changes        :  
*                    
*                    
*                    
******************************************************************************/

/******************************************************************************
*  END OF FILE    :  ser2.h
******************************************************************************/
