
/******************************************************************************
*  File           :  ser3.c
*  Version        :  1
*  Last Changes   :  2019-05-12 11:06:06
******************************************************************************/

/******************************************************************************
*  Include Files
******************************************************************************/

#include <string.h>
#include "typ.h"
#include "ser3.h"

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

enum tyen_Operation ser3_Operation = PROOF_STX;

TUINT8 ser3_au8SndBuf[SER3_SNDBUF_QUELEN + 1];
TUINT16 ser3_u16SndBuf_Head = 0;
TUINT16 ser3_u16SndBuf_Tail = 0;

TUINT8 ser3_au8RecBuf[SER3_RECBUF_QUELEN + 1];
TUINT16 ser3_u16RecBuf_Head = 0;
TUINT16 ser3_u16RecBuf_Tail = 0;

TUINT8 ser3_u8Valid_Data;

/******************************************************************************
*  Global Variables
******************************************************************************/

// Sending
TUINT8 SER3_u8SendPollingRunning; 				// Indicates the polling state if polling is used. For DMA transfer not relevant
TUINT8 SER3_au8SndBuf[SER3_SNDBUF_LEN]; 		// Frame with payload to send
TUINT8 SER3_TransmitFrameLength;				// For DMA transfer

// Receiving
TUINT8 SER3_au8RecBuf[SER3_RECBUF_LEN]; 		// Extracted payload of received frame
TUINT8 SER3_u8ReceivedPayloadLength; 					// Number of data in received payload

/******************************************************************************
*  Local Function Prototypes
******************************************************************************/

void ser3_Start_Sending (void);
TUINT8 ser3_XORChecksum(TUINT8* pu8Data, TUINT8 u8Len);

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

void SER3_Ini (void)
{
  SER3_u8SendPollingRunning = 0;
  ser3_Operation = PROOF_STX;
  ser3_u8Valid_Data = 0;
}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

TUINT8 ser3_XORChecksum(TUINT8* pu8Data, TUINT8 u8Len)
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

void SER3_Protocol_Handling(TUINT8 u8Cycles)
{
  static TUINT8 u8Temp;
  static TUINT8 u8DLC;
  static TUINT8 u8Counter;
  static TUINT8 u8Chk;
  static TUINT8 u8i;
  int i;

  for (i = 0; i < u8Cycles; i++)
  {
    if ((!SER3_u8RecBuf_Empty()) && (!ser3_u8Valid_Data))
    {
      switch (ser3_Operation)
      {
        case PROOF_STX:
        {
          u8Temp = SER3_u8RecBuf_Get();

          if (u8Temp == SER3_STX) // Ist das Startzeichen korrekt?
          {
            ser3_Operation = STORE_DLC;
          }

          break;
        }

        case STORE_DLC:
        {
          u8DLC = SER3_u8RecBuf_Get(); // DLC speichern
          u8Counter = 0; // Wichtig, nicht vergessen, hat schon oft Sorgen bereitet
          ser3_Operation = STORE_DATA;

          break;
        }

        case STORE_DATA: // Daten speichern
        {
          if (u8Counter < u8DLC)
          {
            SER3_au8RecBuf[u8Counter] = SER3_u8RecBuf_Get();
            u8Counter++;
          }
          else
          {
      	    ser3_Operation = PROOF_CHK;
          }

          break;
        }

        case PROOF_CHK: // Checksumme berechnen und mit Checksumme von Protokoll vergleichen
        {
          u8Chk = 0;

          u8Chk ^= SER3_STX;
          u8Chk ^= u8DLC;

          for (u8i = 0; u8i < u8DLC; u8i++)
          {
            u8Chk ^=  SER3_au8RecBuf[u8i];
          }

          u8Temp = SER3_u8RecBuf_Get(); // Hole die Checksumme

          if (u8Temp == u8Chk) // �berpr�fe die Checksumme
          {
            ser3_Operation = PROOF_ETX;
          }
          else
          {
            ser3_Operation = PROOF_STX; // Versuch verwerfen
          }

          break;
        }

        case PROOF_ETX:
        {
          u8Temp = SER3_u8RecBuf_Get();

         if (u8Temp == SER3_ETX) // Ist das Ende Zeichen korrekt?
          {
            ser3_u8Valid_Data = 1; // Jetzt sind die Daten g�ltig ansonsten nicht
            SER3_u8ReceivedPayloadLength = u8DLC; // DLC einer globalen Variable �bergeben
            ser3_Operation = PROOF_STX; // Von vorne anfangen
          }
          else
          {
            ser3_Operation = PROOF_STX; // Von vorne anfangen
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

TUINT8 SER3_u8Valid_Data(void)
{
  TUINT8 u8Ret_Val;

  if (ser3_u8Valid_Data)
  {
    ser3_u8Valid_Data = 0;
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

void SER3_Send_Protocol (TUINT8* pu8Data, TUINT8 u8Len)
{
  // STX, DLC, DATA
  SER3_au8SndBuf[0] = SER3_STX;
  SER3_au8SndBuf[1] = u8Len;
  memcpy((char*)&SER3_au8SndBuf[2], (char*)pu8Data, u8Len);

#ifdef XOR_CHECKSUM
  TUINT8 u8Chk;
  u8Chk = ser3_XORChecksum(SER3_au8SndBuf, u8Len + 2);
#endif


#ifdef SER3_SENDING_MODE_POLLING
  int i;
  SER3_SndBuf_Put(SER3_au8SndBuf[0]);
  SER3_SndBuf_Put(u8Len);
  for (i = 2; i < u8Len + 2; i++) SER3_SndBuf_Put (*(pu8Data + u8i));
  SER3_SndBuf_Put(u8Chk);
  SER3_SndBuf_Put (SER3_ETX);

  // Sendevorgang anstossen
  ser3_Start_Sending();
#endif


#ifdef SER3_SENDING_MODE_DMA
  SER3_au8SndBuf[u8Len + 2] = u8Chk;
  SER3_au8SndBuf[u8Len + 3] = SER3_ETX;

  SER3_TransmitFrameLength = u8Len + 4;
#endif
}


#ifdef SER3_SENDING_MODE_POLLING

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

void SER3_Send_String (TUINT8* pu8Data, TUINT8 u8Len)
{
  TUINT16 u8i;

  // DATA
  for (u8i = 0; u8i < u8Len; u8i++)
  {
	SER3_SndBuf_Put (*(pu8Data + u8i));
  }

  // Sendevorgang anstossen
  ser3_Start_Sending();
}

#endif


/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

#ifdef SER3_SENDING_MODE_POLLING

void ser3_Start_Sending (void)
{
  TUINT8 u8Temp;

  // Sendevorgang ansto�en, wenn gerade nichts gesendet wird
  if (!SER3_u8SendPollingRunning)
  {
	SER3_u8SendPollingRunning = 1;
	u8Temp = SER3_u8SndBuf_Get();
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

void SER3_SndBuf_Put (TUINT8 u8Data)
{
  ser3_au8SndBuf[ser3_u16SndBuf_Tail++] = u8Data;
  if (ser3_u16SndBuf_Tail > SER3_SNDBUF_QUELEN) ser3_u16SndBuf_Tail = 0;
}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

TUINT8 SER3_u8SndBuf_Get (void)
{
  TUINT8 u8Data = ser3_au8SndBuf[ser3_u16SndBuf_Head++];
  if (ser3_u16SndBuf_Head > SER3_SNDBUF_QUELEN) ser3_u16SndBuf_Head = 0;
  return u8Data;
}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

TUINT8 SER3_u8SndBuf_Empty (void)
{
  return ser3_u16SndBuf_Head == ser3_u16SndBuf_Tail;
}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

void SER3_RecBuf_Put (TUINT8 u8Data)
{
  ser3_au8RecBuf[ser3_u16RecBuf_Tail++] = u8Data;
  if (ser3_u16RecBuf_Tail > SER3_RECBUF_QUELEN) ser3_u16RecBuf_Tail = 0;
}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

TUINT8 SER3_u8RecBuf_Get (void)
{
  TUINT8 u8Data = ser3_au8RecBuf[ser3_u16RecBuf_Head++];
  if (ser3_u16RecBuf_Head > SER3_RECBUF_QUELEN) ser3_u16RecBuf_Head = 0;
  return u8Data;
}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

TUINT8 SER3_u8RecBuf_Empty (void)
{
  return ser3_u16RecBuf_Head == ser3_u16RecBuf_Tail;
}


/******************************************************************************
*  Changes        :  
*                    
*                    
*                    
******************************************************************************/

/******************************************************************************
*  END OF FILE    :  ser3.h
******************************************************************************/
