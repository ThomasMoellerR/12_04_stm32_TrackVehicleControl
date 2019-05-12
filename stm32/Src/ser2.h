
/******************************************************************************
*  File           :  ser2.h
*  Version        :  1
*  Last Changes   :  2019-05-12 11:06:06
******************************************************************************/

#ifndef __SER2_H
#define __SER2_H

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

//#define SER2_SENDING_MODE_POLLING             // Define it for mode polling
#define SER2_SENDING_MODE_DMA                   // Define it for mode DMA

#define XOR_CHECKSUM                            // Define it for XOR checksum

#define SER2_STX                        0x65    // Start sign "A"
#define SER2_ETX                        0x0D    // End sign (Carriage Return)

#define SER2_SNDBUF_QUELEN              256     // Length of send queue (Polling mode)
#define SER2_RECBUF_QUELEN              256     // Length of rec queue

#define SER2_SNDBUF_LEN                 1024    // Length of buffer for frame with payload to send
#define SER2_RECBUF_LEN                 1024    // Length of payload buffer

/******************************************************************************
*  Global Type Definitions
******************************************************************************/

/******************************************************************************
*  Global Variables
******************************************************************************/

// Sending
extern TUINT8 SER2_u8SendPollingRunning; 				// Indicates the polling state if polling is used. For DMA transfer not relevant
extern TUINT8 SER2_au8SndBuf[SER2_SNDBUF_LEN]; 		// Frame with payload to send
extern TUINT8 SER2_TransmitFrameLength;				// For DMA transfer

// Receiving
extern TUINT8 SER2_au8RecBuf[SER2_RECBUF_LEN]; 		// Extracted payload of received frame
extern TUINT8 SER2_u8ReceivedPayloadLength; 					// Number of data in received payload


/******************************************************************************
*  Global Function Prototypes
******************************************************************************/

void SER2_Ini (void);

// Sending
void SER2_Send_Protocol (TUINT8* pu8Data, TUINT8 u8Len);	// Daten als Protokoll senden
void SER2_Send_String (TUINT8* pu8Data, TUINT8 u8Len); 		// Daten als roher String senden

// Receiving
TUINT8 SER2_u8Valid_Data(void); 							// Valid payload available?
void SER2_Protocol_Handling(TUINT8 u8Cycles); 				// Processing of the state machine for extracting the payload out of frame

// Ringbuffers
void SER2_SndBuf_Put (TUINT8 u8Data); 						// Send Queue
TUINT8 SER2_u8SndBuf_Get (void); 							// Send Queue
TUINT8 SER2_u8SndBuf_Empty (void); 							// Send Queue

void SER2_RecBuf_Put (TUINT8 u8Data); 						// Rec Queue
TUINT8 SER2_u8RecBuf_Get (void); 							// Rec Queue
TUINT8 SER2_u8RecBuf_Empty (void); 							// Rec Queue

/******************************************************************************
*  Changes        :  
*                    
*                    
*                    
******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __SER2_H */

/******************************************************************************
*  END OF FILE    :  ser2.h
******************************************************************************/
