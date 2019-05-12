
/******************************************************************************
*  File           :  ser3.h
*  Version        :  1
*  Last Changes   :  2019-05-12 11:06:06
******************************************************************************/

#ifndef __SER3_H
#define __SER3_H

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

//#define SER3_SENDING_MODE_POLLING             // Define it for mode polling
#define SER3_SENDING_MODE_DMA                   // Define it for mode DMA

#define XOR_CHECKSUM                            // Define it for XOR checksum

#define SER3_STX                        0x65    // Start sign "A"
#define SER3_ETX                        0x0D    // End sign (Carriage Return)

#define SER3_SNDBUF_QUELEN              256     // Length of send queue (Polling mode)
#define SER3_RECBUF_QUELEN              256     // Length of rec queue

#define SER3_SNDBUF_LEN                 1024    // Length of buffer for frame with payload to send
#define SER3_RECBUF_LEN                 1024    // Length of payload buffer

/******************************************************************************
*  Global Type Definitions
******************************************************************************/

/******************************************************************************
*  Global Variables
******************************************************************************/

// Sending
extern TUINT8 SER3_u8SendPollingRunning; 				// Indicates the polling state if polling is used. For DMA transfer not relevant
extern TUINT8 SER3_au8SndBuf[SER3_SNDBUF_LEN]; 		// Frame with payload to send
extern TUINT8 SER3_TransmitFrameLength;				// For DMA transfer

// Receiving
extern TUINT8 SER3_au8RecBuf[SER3_RECBUF_LEN]; 		// Extracted payload of received frame
extern TUINT8 SER3_u8ReceivedPayloadLength; 					// Number of data in received payload


/******************************************************************************
*  Global Function Prototypes
******************************************************************************/

void SER3_Ini (void);

// Sending
void SER3_Send_Protocol (TUINT8* pu8Data, TUINT8 u8Len);	// Daten als Protokoll senden
void SER3_Send_String (TUINT8* pu8Data, TUINT8 u8Len); 		// Daten als roher String senden

// Receiving
TUINT8 SER3_u8Valid_Data(void); 							// Valid payload available?
void SER3_Protocol_Handling(TUINT8 u8Cycles); 				// Processing of the state machine for extracting the payload out of frame

// Ringbuffers
void SER3_SndBuf_Put (TUINT8 u8Data); 						// Send Queue
TUINT8 SER3_u8SndBuf_Get (void); 							// Send Queue
TUINT8 SER3_u8SndBuf_Empty (void); 							// Send Queue

void SER3_RecBuf_Put (TUINT8 u8Data); 						// Rec Queue
TUINT8 SER3_u8RecBuf_Get (void); 							// Rec Queue
TUINT8 SER3_u8RecBuf_Empty (void); 							// Rec Queue

/******************************************************************************
*  Changes        :  
*                    
*                    
*                    
******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __SER3_H */

/******************************************************************************
*  END OF FILE    :  ser3.h
******************************************************************************/
