
/******************************************************************************
*  File           :  typ.h
*  Version        :  1
*  Last Changes   :  2019-05-12 11:01:11
******************************************************************************/

#ifndef __TYP_H
#define __TYP_H

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

#define TYP_U8	0
#define TYP_S8	1
#define TYP_U16	2
#define TYP_S16	3
#define TYP_U32	4
#define TYP_S32	5
#define TYP_STRING	6
#define TYP_DOUBLE	7
#define TYP_FLOAT	8

/******************************************************************************
*  Global Type Definitions
******************************************************************************/

typedef unsigned char	TUINT8;
typedef signed char		TINT8;
typedef unsigned short	TUINT16;
typedef	short			TINT16;
typedef unsigned int	TUINT32;
typedef int				TINT32;

typedef union
{
  TUINT8	u8Data;
  TINT8		s8Data;
  TUINT16   u16Data;
  TINT16	s16Data;
  TUINT32	u32Data;
  TINT32	s32Data;
  TUINT8	au8Data[8];
  double	dDouble;
  float		fFloat;
  int		iInteger;
} tyun_AnyData;


typedef union
{
	TUINT8 Bit0 : 1;
	TUINT8 Bit1 : 1;
	TUINT8 Bit2 : 1;
	TUINT8 Bit3 : 1;
	TUINT8 Bit4 : 1;
	TUINT8 Bit5 : 1;
	TUINT8 Bit6 : 1;
	TUINT8 Bit7 : 1;
	TUINT8 Bit8 : 1;
	TUINT8 Bit9 : 1;
	TUINT8 Bit10 : 1;
	TUINT8 Bit11 : 1;
	TUINT8 Bit12 : 1;
	TUINT8 Bit13 : 1;
	TUINT8 Bit14 : 1;
	TUINT8 Bit15 : 1;
}tyun_16BitData;

/******************************************************************************
*  Global Variables
******************************************************************************/

/******************************************************************************
*  Global Function Prototypes
******************************************************************************/

/******************************************************************************
*  Changes        :  
*                    
*                    
*                    
******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __TYP_H */

/******************************************************************************
*  END OF FILE    :  typ.h
******************************************************************************/
