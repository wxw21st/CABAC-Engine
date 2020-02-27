/*****************************************************************************
 * bitstream.h: Bitstream declarations
 *****************************************************************************
 *
 * Authors: Xiangwen Wang
 *          wxw21st@gmail.com.
 * Time: init at 2015-03-25, 
 * Addr: Leitch Lab of University of Waterloo 
 *****************************************************************************/

#ifndef __BS_H__
#define __BS_H__

#include "config.h"
#include "utils.h"
#include "assert.h"
// ***************************************************************************
typedef struct xBitStream {
	uint8_t      *pucBits0;
	uint8_t      *pucBits;
    uint32_t      dwCache;
     int32_t      nCachedBits;

	 int32_t	  iNumOfStreamBytes;

	//used for dec
	 int32_t	  iFifoIdx;
	 int32_t	  iNumOfBitsHeld;
	 uint32_t	  uiHeldBitsValue;
} xBitStream;


#define putBits32(dst, x)    *(uint32_t*)(dst) = (x);

#define flushCache(dst, x, bits)    { \
	int _i; \
	for(_i=0; _i < (bits)>>3; _i++) { \
		const uint8_t _tmp = (x) >> 24; \
		(x) <<= 8; \
		*(dst)++ = _tmp;		\
	} \
}


//encoder interface
void	xWriteAlignZero(xBitStream *);
void	xWriteCode( xBitStream *, uint32_t , uint32_t );
int32_t xBitFlush(xBitStream *);

//encoder interface
void	xInputBitstreamInit( xBitStream * );
uint32_t readByte( xBitStream *);
void	xBitStreamInit(xBitStream *, uint8_t *, int32_t );


#endif /* __BS_H__ */
