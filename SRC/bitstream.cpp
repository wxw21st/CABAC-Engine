
/*****************************************************************************
 * bitstream.cpp: Bitstream functions
 *****************************************************************************
 *
 * Authors: Xiangwen Wang
 *          wxw21st@gmail.com.
 * Time: Init at 2015-03-25, 
 * Addr: Leitch Lab of University of Waterloo 
 *****************************************************************************/

#include "config.h"
#include "assert.h"
#include "utils.h"
#include "bitstream.h"


// ***************************************************************************
void xBitStreamInit(xBitStream *pBS, uint8_t *pucBuffer, int32_t nBufferSize)
{
	assert( nBufferSize > 0 );
	pBS->pucBits0       = 
	pBS->pucBits        = pucBuffer;

	pBS->dwCache        = 0;
	pBS->nCachedBits    = 0;

	pBS->iFifoIdx		= 0;
	pBS->uiHeldBitsValue= 0;
	pBS->iNumOfBitsHeld	= 0;
}

uint32_t read( xBitStream *pBS, int32_t iNumOfBitsRead)
{
	assert( iNumOfBitsRead <= 32 );

	 int32_t  iNumOfBitsHeld	= pBS->iNumOfBitsHeld;
	uint32_t uiHeldBitsValue	= pBS->uiHeldBitsValue;

	/* NB, bits are extracted from the MSB of each byte. */
	uint32_t retval = 0;
	if (iNumOfBitsRead <= iNumOfBitsHeld)
	{
		/* n=1, len(H)=7:   -VHH HHHH, shift_down=6, mask=0xfe
		 * n=3, len(H)=7:   -VVV HHHH, shift_down=4, mask=0xf8
		 */
		retval  = uiHeldBitsValue >> (iNumOfBitsHeld - iNumOfBitsRead);
		retval &= ~(0xff << iNumOfBitsRead);
		iNumOfBitsHeld -= iNumOfBitsRead;

		pBS->iNumOfBitsHeld = iNumOfBitsHeld;

		return	retval;
     }

  /* all num_held_bits will go into retval
   *   => need to mask leftover bits from previous extractions
   *   => align retval with top of extracted word */
  /* n=5, len(H)=3: ---- -VVV, mask=0x07, shift_up=5-3=2,
   * n=9, len(H)=3: ---- -VVV, mask=0x07, shift_up=9-3=6 */
	iNumOfBitsRead -= iNumOfBitsHeld;
	retval   = uiHeldBitsValue & ~(0xff << iNumOfBitsHeld);
	retval <<= iNumOfBitsRead;

  /* number of whole bytes that need to be loaded to form retval */
  /* n=32, len(H)=0, load 4bytes, shift_down=0
   * n=32, len(H)=1, load 4bytes, shift_down=1
   * n=31, len(H)=1, load 4bytes, shift_down=1+1
   * n=8,  len(H)=0, load 1byte,  shift_down=0
   * n=8,  len(H)=3, load 1byte,  shift_down=3
   * n=5,  len(H)=1, load 1byte,  shift_down=1+3
   */
	uint32_t aligned_word = 0;
	int32_t  num_bytes_to_load = (iNumOfBitsRead - 1) >> 3;
	assert( pBS->iFifoIdx + num_bytes_to_load < pBS->iNumOfStreamBytes);

	switch (num_bytes_to_load)
	{
	case 3: aligned_word  = (pBS->pucBits)[pBS->iFifoIdx++] << 24;
	case 2: aligned_word |= (pBS->pucBits)[pBS->iFifoIdx++] << 16;
	case 1: aligned_word |= (pBS->pucBits)[pBS->iFifoIdx++] <<  8;
	case 0: aligned_word |= (pBS->pucBits)[pBS->iFifoIdx++];
	}

  /* resolve remainder bits */
	int32_t next_num_held_bits = (32 - iNumOfBitsRead) % 8;

  /* copy required part of aligned_word into retval */
	retval |= aligned_word >> next_num_held_bits;

  /* store held bits */
	pBS->iNumOfBitsHeld = next_num_held_bits;
	pBS->uiHeldBitsValue = aligned_word;

	return retval;
}

uint32_t readByte( xBitStream *pBS )
{
	//assert(pBS->iFifoIdx < pBS->iNumOfStreamBytes);
	return (pBS->pucBits)[pBS->iFifoIdx++];
}


void xPutBits(xBitStream *pBS, uint32_t uiBits, int32_t nNumBits)
{
	int32_t nShift = 32 - pBS->nCachedBits - nNumBits;

	assert((nNumBits >= 0) && (nNumBits <= 32));
	assert(xSHR(uiBits, nNumBits) == 0);

	if (nShift >= 0) {
		pBS->dwCache     |= xSHL(uiBits, nShift);
		pBS->nCachedBits += nNumBits;
	}
	else {
		uint32_t dwCache = pBS->dwCache;
		dwCache |= xSHR(uiBits, -nShift);
		flushCache(pBS->pucBits, dwCache, 32);
		pBS->dwCache = xSHL(uiBits, (32 + nShift));
		pBS->nCachedBits = -nShift;
	}
}

void xWriteAlignZero(xBitStream *pBS)
{
	int32_t nShift = (8 - pBS->nCachedBits) & 7;
	xPutBits(pBS, 0, nShift);
}

int32_t xBitFlush(xBitStream *pBS)
{
	flushCache(pBS->pucBits, pBS->dwCache, pBS->nCachedBits);

	pBS->nCachedBits &= 7;
	return (int32_t)( ( pBS->pucBits - pBS->pucBits0 ) + ( pBS->nCachedBits + 7) / 8 );
}


// ***************************************************************************
void xWriteCode( xBitStream *pBS, uint32_t uiCode, uint32_t uiLength )
{
	assert ( uiLength > 0 );
	xPutBits( pBS, uiCode, uiLength );
}

void xWriteUvlc( xBitStream *pBS, uint32_t uiCode )
{
	uint32_t uiLength = xLog2(++uiCode) - 1;

	xPutBits( pBS, 0,       uiLength);
	xPutBits( pBS, uiCode, (uiLength+1));
}

uint32_t xConvertToUInt( int32_t iValue )
{
	return ( iValue > 0) ? (iValue<<1)-1 : -iValue<<1;
}

void xWriteSvlc( xBitStream *pBS, int32_t iCode )
{
	uint32_t uiCode;

	uiCode = xConvertToUInt( iCode );
	xWriteUvlc( pBS, uiCode );
}

void xWriteFlag( xBitStream *pBS, uint32_t uiCode )
{
	xPutBits( pBS, uiCode, 1 );
}

