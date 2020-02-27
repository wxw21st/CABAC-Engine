/*****************************************************************************
 * cabac.h: Bitstream functions
 *****************************************************************************
 *
 * Authors: Xiangwen Wang
 *          wxw21st@gmail.com.
 * Time: Init at 2015-03-25, 
 * Addr: Leitch Lab of University of Waterloo 
 *****************************************************************************/

#ifndef __CABAC_H__
#define __CABAC_H__

#include "table.h"
#include "bitstream.h"
#include "config.h"



typedef struct {
	// Engine
	int32_t	uiLow;
	int32_t	uiRange;
	int32_t	iBitsLeft;
	int8_t	ucCache;
	int32_t	uiNumBytes;
	
	//UInt                m_uiRange; -->uiRange
	//UInt                m_uiValue; -->uiLow
	int32_t             iBitsNeeded;

	// Context Model
	uint8_t   contextModels[MAX_NUM_CTX_MOD];

} xCabac;

typedef struct{
	xSliceType	eSliceType;
	int32_t		iQP;
	int8_t		iCtxIdx;
	uint32_t	uiTerminalBin;

	int32_t		iNumOfInUnit;
}xContext;

typedef struct xMV {
	int16_t   x;
	int16_t   y;
} xMV;

#define CABAC_ENTER								\
	uint32_t  uiLow		= pCabac->uiLow;		\
	uint32_t  uiRange	= pCabac->uiRange;		\
	 int32_t  iBitsLeft = pCabac->iBitsLeft;	\
	uint8_t   ucCache	= pCabac->ucCache;		\
	 int32_t  uiNumBytes = pCabac->uiNumBytes;	\
	 int32_t  iBitsNeeded = pCabac->iBitsNeeded;

#define CABAC_LEAVE						\
	pCabac->uiLow		= uiLow;		\
	pCabac->uiRange		= uiRange;		\
	pCabac->iBitsLeft	= iBitsLeft;	\
	pCabac->ucCache		= ucCache;		\
	pCabac->uiNumBytes	= uiNumBytes;	\
	pCabac->iBitsNeeded = iBitsNeeded;

#define GetMPS( state )     ( (uint32_t)(state) &  1 )
#define GetState( state )   ( (state) >> 1 )
#define UpdateLPS( state )  ( (state) = xg_aucNextStateLPS[ (state) ] )
#define UpdateMPS( state )  ( (state) = xg_aucNextStateMPS[ (state) ] )

//void xCabacEncodeBin( xCabac *pCabac, uint32_t binValue, uint32_t  );
//void xCabacEncodeTerminatingBit( xCabac *pCabac, uint32_t binValue );
//void testAndWriteOut( xCabac *pCabac, xBitStream *pBS );
//void xCabacFlush( xCabac *pCabac, xBitStream *pBS );

//void xCabacDecodeBinTrm( xCabac *pCabac, uint32_t *binValue, uint32_t , xBitStream * pBS );
//void xCabacDecodeBin( xCabac *pCabac, uint32_t *binValue, uint32_t , xBitStream * pBS );
//void xCabacDecStart( xCabac * pCabac, xBitStream * pBS );

void xCabacReset(		xCabac * );
void xCabacInit(		xCabac *, xSliceType eSliceType, cint32_t iQP );
void xCabacEncode_test( xCabac *, xBitStream *, uint8_t *,	xContext*);
void xCabacDecode_test( xCabac *, xBitStream *, uint8_t *,	xContext*);
void xDecompressMvd(	xCabac *, xBitStream *, xMV *,		xContext * );
void xCompressMvd(		xCabac *, xBitStream *, xMV *,		xContext * );

#endif