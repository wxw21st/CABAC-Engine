/*****************************************************************************
 * encode.cpp: cabac encode functions
 *****************************************************************************
 *
 * Authors: Xiangwen Wang
 *          wxw21st@gmail.com.
 * Time: init at 2015-03-25, 
 * Addr: Leitch Lab of University of Waterloo 
 *****************************************************************************/


#include <stdio.h>
#include "bitstream.h"
#include "cabac.h"
#include "config.h"
#include <math.h>


// ***************************************************************************
#define WRITE_CODE( value, length, name)     xWriteCode ( pBS, value, length )
#define WRITE_UVLC( value,         name)     xWriteUvlc ( pBS, value )
#define WRITE_SVLC( value,         name)     xWriteSvlc ( pBS, value )
#define WRITE_FLAG( value,         name)     xWriteFlag ( pBS, value )




void xCabacInitEntry( int32_t items, cint32_t qp, uint8_t *pucState, cuint8_t *pInitValue )
{
	int32_t i;
	assert( (qp >= 0) && (qp <= 51) );

	for( i=0; i<items; i++ ) {
		int32_t initValue	= pInitValue[i];
		// [9.2.1.1]
		int32_t  slopeIdx   = ( initValue >> 4);
		int32_t  intersecIdx= ( initValue & 15 );
		int32_t  m          =   slopeIdx * 5 - 45;
		int32_t  n          = ( intersecIdx << 3 ) - 16;
		int32_t  initState  =  Clip3( 1, 126, ( ( ( m * qp ) >> 4 ) + n ) );
		uint32_t valMPS     = (initState >= 64 );
		pucState[i]			= ( (valMPS ? (initState - 64) : (63 - initState)) <<1) + valMPS;
	}
}

void xCabacInit( xCabac *pCabac, xSliceType eSliceType, cint32_t iQp )
{
	//CUInt		 eSlice		= 0;
	//cint32_t	 iQp		= 22;//qp value
	uint8_t		*pucState	= pCabac->contextModels;
	int32_t		 nOffset	= 0;

#define INIT_CABAC( n, m, v ) \
	xCabacInitEntry( (m)*(n), iQp, pucState, (v)[eSliceType] ); \
	pucState += (n)*(m); \
	nOffset  += (n)*(m);

	assert( nOffset == OFF_SPLIT_FLAG_CTX );
	INIT_CABAC( 1, NUM_SPLIT_FLAG_CTX,          INIT_SPLIT_FLAG         );
	assert( nOffset == OFF_SKIP_FLAG_CTX );
	INIT_CABAC( 1, NUM_SKIP_FLAG_CTX,           INIT_SKIP_FLAG          );
	assert( nOffset == OFF_MERGE_FLAG_EXT_CTX );
	INIT_CABAC( 1, NUM_MERGE_FLAG_EXT_CTX,      INIT_MERGE_FLAG_EXT     );
	assert( nOffset == OFF_MERGE_IDX_EXT_CTX );
	INIT_CABAC( 1, NUM_MERGE_IDX_EXT_CTX,       INIT_MERGE_IDX_EXT      );
	assert( nOffset == OFF_PART_SIZE_CTX );
	INIT_CABAC( 1, NUM_PART_SIZE_CTX,           INIT_PART_SIZE          );
	assert( nOffset == OFF_CU_AMP_CTX );
	INIT_CABAC( 1, NUM_CU_AMP_CTX,              INIT_CU_AMP_POS         );
	assert( nOffset == OFF_PRED_MODE_CTX );
	INIT_CABAC( 1, NUM_PRED_MODE_CTX,           INIT_PRED_MODE          );
	assert( nOffset == OFF_INTRA_PRED_CTX );
	INIT_CABAC( 1, NUM_ADI_CTX,                 INIT_INTRA_PRED_MODE    );
	assert( nOffset == OFF_CHROMA_PRED_CTX );
	INIT_CABAC( 1, NUM_CHROMA_PRED_CTX,         INIT_CHROMA_PRED_MODE   );
	assert( nOffset == OFF_INTER_DIR_CTX );
	INIT_CABAC( 1, NUM_INTER_DIR_CTX,           INIT_INTER_DIR          );
	assert( nOffset == OFF_MVD_CTX );
	INIT_CABAC( 1, NUM_MV_RES_CTX,              INIT_MVD                );
	assert( nOffset == OFF_REF_PIC_CTX );
	INIT_CABAC( 1, NUM_REF_NO_CTX,              INIT_REF_PIC            );
	assert( nOffset == OFF_DELTA_QP_CTX );
	INIT_CABAC( 1, NUM_DELTA_QP_CTX,            INIT_DQP                );
	assert( nOffset == OFF_QT_CBF_CTX );
	INIT_CABAC( 2, NUM_QT_CBF_CTX,              INIT_QT_CBF             );
	assert( nOffset == OFF_QT_ROOT_CBF_CTX );
	INIT_CABAC( 1, NUM_QT_ROOT_CBF_CTX,         INIT_QT_ROOT_CBF        );
	assert( nOffset == OFF_SIG_CG_FLAG_CTX );
	INIT_CABAC( 2, NUM_SIG_CG_FLAG_CTX,         INIT_SIG_CG_FLAG        );
	assert( nOffset == OFF_SIG_FLAG_CTX );
	INIT_CABAC( 1, NUM_SIG_FLAG_CTX,            INIT_SIG_FLAG           );
	assert( nOffset == OFF_LAST_X_CTX );
	INIT_CABAC( 2, NUM_LAST_FLAG_XY_CTX,        INIT_LAST               );
	assert( nOffset == OFF_LAST_Y_CTX );
	INIT_CABAC( 2, NUM_LAST_FLAG_XY_CTX,        INIT_LAST               );
	assert( nOffset == OFF_ONE_FLAG_CTX );
	INIT_CABAC( 1, NUM_ONE_FLAG_CTX,            INIT_ONE_FLAG           );
	assert( nOffset == OFF_ABS_FLAG_CTX );
	INIT_CABAC( 1, NUM_ABS_FLAG_CTX,            INIT_ABS_FLAG           );
	assert( nOffset == OFF_MVP_IDX_CTX );
	INIT_CABAC( 1, NUM_MVP_IDX_CTX,             INIT_MVP_IDX            );
	assert( nOffset == OFF_TRANS_SUBDIV_FLAG_CTX );
	INIT_CABAC( 1, NUM_TRANS_SUBDIV_FLAG_CTX,   INIT_TRANS_SUBDIV_FLAG  );
	assert( nOffset == OFF_TS_FLAG_CTX );
	INIT_CABAC( 1, NUM_TRANSFORMSKIP_FLAG_CTX,  INIT_TRANSFORMSKIP_FLAG );
	assert( nOffset == OFF_SAO_MERGE_FLAG_CTX );
	INIT_CABAC( 1, NUM_SAO_MERGE_FLAG_CTX,      INIT_SAO_MERGE_FLAG     );
	assert( nOffset == OFF_SAO_TYPE_IDX_CTX );
	INIT_CABAC( 1, NUM_SAO_TYPE_IDX_CTX,        INIT_SAO_TYPE_IDX       );

#undef INIT_CABAC
	assert( nOffset < MAX_NUM_CTX_MOD );
}

void xCabacFlush( xCabac *pCabac, xBitStream *pBS )
{
	CABAC_ENTER;
	if ( uiLow >> (32 - iBitsLeft) ) {
		//assert( uiNumBytes > 0 );
		//assert( ucCache != 0xff );
		WRITE_CODE( ucCache + 1, 8, "xCabacFlush0" );
		while( uiNumBytes > 1 ) {
			WRITE_CODE( 0x00, 8, "xCabacFlush1" );
			uiNumBytes--;
		}
		uiLow -= 1 << ( 32 - iBitsLeft );
	}
	else  {
		if ( uiNumBytes > 0 ) {
			WRITE_CODE( ucCache, 8, "xCabacFlush2" );
		}
		while ( uiNumBytes > 1 ) {
			WRITE_CODE( 0xFF, 8, "xCabacFlush3" );
			uiNumBytes--;
		}
	}
	WRITE_CODE( uiLow >> 8, 24 - iBitsLeft, "xCabacFlush4" );
	CABAC_LEAVE;
}

void xCabacReset( xCabac *pCabac )
{
	pCabac->uiLow         = 0;
	pCabac->uiRange       = 510;
	pCabac->iBitsLeft     = 23;
	pCabac->ucCache       = 0xFF;
	pCabac->uiNumBytes    = 0;
}


void testAndWriteOut( xCabac *pCabac, xBitStream *pBS )
{
	CABAC_ENTER;
	if( iBitsLeft < 12 ) {
		uint32_t leadByte = uiLow >> (24 - iBitsLeft);
		iBitsLeft += 8;
		uiLow     &= 0xFFFFFFFF >> iBitsLeft;

		if ( leadByte == 0xff ) {
			uiNumBytes++;
		}
		else {
			if ( uiNumBytes > 0 ) {
				uint32_t carry = leadByte >> 8;
				uint32_t byte = ucCache + carry;
				ucCache = leadByte & 0xff;					//clear carry, saved for next output!
				WRITE_CODE( byte, 8, "testAndWriteOut0" );

				byte = ( 0xff + carry ) & 0xff;
				while ( uiNumBytes > 1 ) {
					WRITE_CODE( byte, 8, "testAndWriteOut0" );
					uiNumBytes--;
				}
			}
			else {
				uiNumBytes = 1;
				ucCache = leadByte;
			}
		}
	}
	CABAC_LEAVE;
}

void xCabacEncodeTerminatingBit( xCabac *pCabac, uint32_t binValue )
{
	CABAC_ENTER;

	uiRange -= 2;
	if( binValue ) {
		uiLow  += uiRange;
		uiLow <<= 7;
		//uiRange = 2 << 7;
		iBitsLeft -= 7;
	}
	else if ( uiRange < 256 ) {
		uiLow   <<= 1;
		uiRange <<= 1;
		iBitsLeft--;
	}
	CABAC_LEAVE;
}

void xCabacEncodeBinEP( xCabac *pCabac, xBitStream *pBS, uint32_t binValues, int32_t numBins )
{
	pCabac->uiLow <<= 1;
	if( binValues ) {
		pCabac->uiLow += pCabac->uiRange;
	}
	pCabac->iBitsLeft--;

	testAndWriteOut( pCabac, pBS );

}

void xCabacEncodeBinsEP( xCabac *pCabac, xBitStream *pBS, uint32_t binValues, int32_t numBins )
{
	assert( binValues < (uint32_t)(1 << numBins) );
	while ( numBins > 8 ) {
		numBins -= 8;
		uint32_t pattern = binValues >> numBins;
		pCabac->uiLow <<= 8;
		pCabac->uiLow += pCabac->uiRange * pattern;
		binValues -= pattern << numBins;
		pCabac->iBitsLeft -= 8;

		testAndWriteOut( pCabac, pBS );
	}

	pCabac->uiLow <<= numBins;
	pCabac->uiLow  += pCabac->uiRange * binValues;
	pCabac->iBitsLeft -= numBins;
	testAndWriteOut( pCabac, pBS );

}


void xWriteEpExGolomb( xCabac *pCabac, xBitStream *pBS, uint32_t uiSymbol, uint32_t uiCount )
{
	uint32_t bins = 0;
	uint32_t numBins = 0;

	while( (int32_t)uiSymbol >= (int32_t)(1<<uiCount) ) {
		bins = 2 * bins + 1;
		numBins++;
		uiSymbol -= (int32_t)(1 << uiCount);
		uiCount  ++;
	}
	bins = 2 * bins + 0;
	numBins++;

	bins = (bins << uiCount) | uiSymbol;
	numBins += uiCount;

	assert( numBins <= 32 );
	xCabacEncodeBinsEP( pCabac, pBS, bins, numBins );
}



void xCabacEncodeBin( xCabac *pCabac, uint32_t binValue, uint32_t iCtxIdx )
{
	CABAC_ENTER;
    uint8_t ucState	 = pCabac->contextModels[iCtxIdx];
    uint32_t  uiLPS	 = xg_aucLPSTable[ GetState( ucState ) ][ ( uiRange >> 6 ) & 3 ];
    uiRange			-= uiLPS;

    if( binValue != GetMPS(ucState) ) {
        int32_t numBits = xg_aucRenormTable[ uiLPS >> 3 ];
        uiLow     = ( uiLow + uiRange ) << numBits;
        uiRange   =   uiLPS << numBits;
        UpdateLPS( ucState );

        iBitsLeft -= numBits;
    }
    else {
        UpdateMPS( ucState );
        if ( uiRange < 256 ) {
            uiLow <<= 1;
            uiRange <<= 1;
            iBitsLeft--;
        }
    }

    pCabac->contextModels[iCtxIdx] = ucState;
    CABAC_LEAVE;

/*
 *	#define CABAC_LEAVE \
 *	pCabac->uiLow = uiLow; \
 *	pCabac->uiRange = uiRange; \
 *	pCabac->iBitsLeft = iBitsLeft; \
 *	pCabac->ucCache = ucCache; \
 *	pCabac->uiNumBytes = uiNumBytes;
 */
}

void xCabacEncodeBin_withTestOut( xCabac *pCabac, xBitStream * pBS, uint32_t binValue, uint32_t iCtxIdx )
{
	CABAC_ENTER;
    uint8_t ucState	 = pCabac->contextModels[iCtxIdx];
    uint32_t  uiLPS	 = xg_aucLPSTable[ GetState( ucState ) ][ ( uiRange >> 6 ) & 3 ];
    uiRange			-= uiLPS;

    if( binValue != GetMPS(ucState) ) {
        int32_t numBits = xg_aucRenormTable[ uiLPS >> 3 ];
        uiLow     = ( uiLow + uiRange ) << numBits;
        uiRange   =   uiLPS << numBits;
        UpdateLPS( ucState );

        iBitsLeft -= numBits;
    }
    else {
        UpdateMPS( ucState );
        if ( uiRange < 256 ) {
            uiLow <<= 1;
            uiRange <<= 1;
            iBitsLeft--;
        }
    }

    pCabac->contextModels[iCtxIdx] = ucState;
    CABAC_LEAVE;

	testAndWriteOut( pCabac, pBS );

/*
 *	#define CABAC_LEAVE \
 *	pCabac->uiLow = uiLow; \
 *	pCabac->uiRange = uiRange; \
 *	pCabac->iBitsLeft = iBitsLeft; \
 *	pCabac->ucCache = ucCache; \
 *	pCabac->uiNumBytes = uiNumBytes;
 */
}

void xCabacEncode_test( xCabac *pCabac, xBitStream *pBS, uint8_t *aucInBuf, xContext * ph )
{
	int8_t	 iCtxIdx	= ph->iCtxIdx;
	int32_t	 iTerminalBin=ph->uiTerminalBin;
	uint32_t binValue	= 0;
	uint8_t  temp		= 0;


	//begin to encode. Number of bits to encode: MAX_BUF_SIZE * 8
	for ( int i=0; i<ph->iNumOfInUnit; i++) {
		temp = aucInBuf[i];
		for ( int j=0; j<8; j++ ) {
			binValue = (temp >> j) & 1;
			xCabacEncodeBin( pCabac, binValue, iCtxIdx );
			testAndWriteOut( pCabac, pBS );
		}
	}
	xCabacEncodeTerminatingBit( pCabac, iTerminalBin );
	testAndWriteOut( pCabac, pBS );

	xCabacFlush( pCabac, pBS );
	xWriteAlignZero( pBS );

	pBS->iNumOfStreamBytes = xBitFlush(pBS);
}

/*
* Cabac for MVD (Motion Vector Difference)
*/
void xEncMVD( xCabac *pCabac, xBitStream *pBS, xMV mvd)
{
	cint32_t iHor = mvd.x;
	cint32_t iVer = mvd.y;
	CUInt bHorAbsGr0 = iHor != 0;
	CUInt bVerAbsGr0 = iVer != 0;
	CUInt uiHorAbs   = abs(iHor);
	CUInt uiVerAbs   = abs(iVer);

	xCabacEncodeBin_withTestOut( pCabac, pBS, (bHorAbsGr0 ? 1 : 0), OFF_MVD_CTX );
	xCabacEncodeBin_withTestOut( pCabac, pBS, (bVerAbsGr0 ? 1 : 0), OFF_MVD_CTX );

	if( bHorAbsGr0 ) {
		xCabacEncodeBin_withTestOut( pCabac, pBS, (uiHorAbs > 1 ? 1 : 0), OFF_MVD_CTX + 1 );
	}

	if( bVerAbsGr0 ) {
		xCabacEncodeBin_withTestOut( pCabac, pBS, (uiVerAbs > 1 ? 1 : 0), OFF_MVD_CTX + 1 );
	}

	if( bHorAbsGr0 ) {
		if( uiHorAbs > 1 ) {
			xWriteEpExGolomb( pCabac, pBS, uiHorAbs-2, 1 );
		}
		xCabacEncodeBinsEP( pCabac, pBS, (iHor < 0 ? 1 : 0), 1 );
	}

	if( bVerAbsGr0 ) {
		if( uiVerAbs > 1 ) {
			xWriteEpExGolomb( pCabac, pBS, uiVerAbs-2, 1 );
		}

		xCabacEncodeBinsEP( pCabac, pBS,  (iVer < 0 ? 1 : 0), 1 );
	}

}


void xCompressMvd( xCabac *pCabac, xBitStream *pBS, xMV *pMvdBuf, xContext * ph )
{
	int32_t	 iTerminalBin = ph->uiTerminalBin;

	for ( int i=0; i<ph->iNumOfInUnit; i++) {
		xMV tmp_mvd = *(pMvdBuf + i);
		xEncMVD( pCabac, pBS, tmp_mvd );
	}
	xCabacEncodeTerminatingBit( pCabac, iTerminalBin );
	testAndWriteOut( pCabac, pBS );

	xCabacFlush( pCabac, pBS );
	xWriteAlignZero( pBS );

	pBS->iNumOfStreamBytes = xBitFlush(pBS);
}
