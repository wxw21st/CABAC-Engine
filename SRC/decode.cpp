/*****************************************************************************
 * decode.cpp: cabac decode functions
 *****************************************************************************
 *
 * Authors: Xiangwen Wang
 *          wxw21st@gmail.com.
 * Time: Init at 2015-03-25, 
 * Addr: Leitch Lab of University of Waterloo 
 *****************************************************************************/

#include <stdio.h>
#include "bitstream.h"
#include "cabac.h"
#include "config.h"
#include "assert.h"


void xCabacDecStart( xCabac * pCabac, xBitStream * pBS )
{
	//assert( m_pcTComBitstream->getNumBitsUntilByteAligned() == 0 );
	assert( pBS->iNumOfBitsHeld == 0 );
	pCabac->uiRange    = 510;
	pCabac->iBitsNeeded = -8;
	pCabac->uiLow    = (readByte( pBS ) << 8);
	pCabac->uiLow	|=  readByte( pBS );
}


void xCabacDecodeBinTrm( xCabac *pCabac, uint32_t *binValue, uint32_t iCtxIdx, xBitStream * pBS )
{
	CABAC_ENTER;

	uint32_t ruiBin = 0;

	uiRange -= 2;
	uint32_t scaledRange = uiRange << 7;
	if( uiLow >= scaledRange ) {
		ruiBin = 1;
	}
	else
	{
		ruiBin = 0;
		if ( scaledRange < ( 256 << 7 ) ) {
			uiRange = scaledRange >> 6;
			uiLow += uiLow;

			if ( ++iBitsNeeded == 0 ) {
				iBitsNeeded = -8;
				uiLow += readByte( pBS );      
			}
		}
	}
	CABAC_LEAVE;
	*binValue = ruiBin;
}
//void xCabacDecodeBin( uint32_t *ruiBin, ContextModel &rcCtxModel )
void xCabacDecodeBin( xCabac *pCabac, uint32_t *binValue, uint32_t iCtxIdx, xBitStream * pBS )
{
	CABAC_ENTER;
	uint8_t  ucState = pCabac->contextModels[iCtxIdx];
	uint32_t uiLPS	 = xg_aucLPSTable[ GetState( ucState)][ (uiRange >> 6) & 3];
	uiRange			-= uiLPS;
	uint32_t scaledRange = uiRange << 7;

	if ( uiLow < scaledRange ) {
		//MPS path
		*binValue = GetMPS( ucState);
		UpdateMPS( ucState);
		if ( scaledRange >= (256 << 7)) {
			pCabac->contextModels[iCtxIdx] = ucState;
			CABAC_LEAVE;

			return;
		}
		uiRange  = scaledRange >> 6;
		uiLow	+= uiLow;

		if ( ++iBitsNeeded == 0 ) {
			iBitsNeeded = -8;
			uiLow	   += readByte( pBS ); 
		}
		
	}else{
		//LPS path
		int32_t numBits = xg_aucRenormTable[ uiLPS >>3 ];
		uiLow	= ( uiLow - scaledRange) << numBits;
		uiRange = uiLPS << numBits;
		*binValue = 1 - GetMPS( ucState);
		UpdateLPS( ucState );

		iBitsNeeded += numBits;
		if ( iBitsNeeded >= 0 ) {
			uiLow += readByte( pBS ) << iBitsNeeded;
			iBitsNeeded -= 8;
		}
		
	}
	pCabac->contextModels[iCtxIdx] = ucState;
	CABAC_LEAVE;

}

void decodeBinsEP( xCabac *pCabac, uint32_t *binValue, int32_t numBins, xBitStream * pBS )
{
	CABAC_ENTER
	uint32_t bins = 0;

	while ( numBins > 8 ) {
		uiLow = ( uiLow << 8 ) + ( readByte( pBS ) << ( 8 + iBitsNeeded ) );

		uint32_t scaledRange = uiRange << 15;
		for ( int32_t i = 0; i < 8; i++ ) {
			bins += bins;
			scaledRange >>= 1;
			if ( uiLow >= scaledRange ) {
				bins++;
				uiLow -= scaledRange;
			}
		}
		numBins -= 8;
	}

	iBitsNeeded += numBins;
	uiLow <<= numBins;

	if ( iBitsNeeded >= 0 ) {
		uiLow += readByte( pBS ) << iBitsNeeded;
		iBitsNeeded -= 8;
	}

	uint32_t scaledRange = uiRange << ( numBins + 7 );
	for ( int32_t i = 0; i < numBins; i++ ) {
		bins += bins;
		scaledRange >>= 1;
		if ( uiLow >= scaledRange ) {
			bins++;
			uiLow -= scaledRange;
		}
	}
	CABAC_LEAVE

	*binValue = bins;
}


void decodeBinEP( xCabac *pCabac, uint32_t *ruiBin, xBitStream * pBS )
{
	CABAC_ENTER

	uiLow += uiLow;

	if ( ++iBitsNeeded >= 0 ) {
		iBitsNeeded = -8;
		uiLow += readByte( pBS );
	}

	*ruiBin = 0;
	uint32_t scaledRange = uiRange << 7;
	if ( uiLow >= scaledRange ) {
		*ruiBin = 1;
		uiLow -= scaledRange;
	}
	CABAC_LEAVE

}

//void decodeBinsEP( xCabac *pCabac, uint32_t *binValue, int32_t numBins, xBitStream * pBS )
void xReadEpExGolomb( xCabac *pCabac, uint32_t *ruiSymbol, uint32_t uiCount, xBitStream * pBS )
{
	uint32_t uiSymbol = 0;
	uint32_t uiBit = 1;

	while( uiBit ) {
		decodeBinEP( pCabac, &uiBit, pBS );
		uiSymbol += uiBit << uiCount++;
	}

	if ( --uiCount ) {
		uint32_t bins;
		decodeBinsEP( pCabac, &bins, uiCount, pBS );
		uiSymbol += bins;
	}
	*ruiSymbol = uiSymbol;
}

void parseMvd( xCabac *pCabac, xBitStream *pBS, xMV *mvOut )
{
	uint32_t uiSymbol;
	uint32_t uiHorAbs;
	uint32_t uiVerAbs;
	uint32_t uiHorSign = 0;
	uint32_t uiVerSign = 0;

	{
		xCabacDecodeBin( pCabac, &uiHorAbs, OFF_MVD_CTX, pBS );
		xCabacDecodeBin( pCabac, &uiVerAbs, OFF_MVD_CTX, pBS );

		const bool bHorAbsGr0 = uiHorAbs != 0;
		const bool bVerAbsGr0 = uiVerAbs != 0;

		if( bHorAbsGr0 ) {
			xCabacDecodeBin( pCabac, &uiSymbol, OFF_MVD_CTX+1, pBS );
			uiHorAbs += uiSymbol;
		}

		if( bVerAbsGr0 ) {
			xCabacDecodeBin( pCabac, &uiSymbol, OFF_MVD_CTX+1, pBS );
			uiVerAbs += uiSymbol;
		}

		if( bHorAbsGr0 ) {
			if( 2 == uiHorAbs ) {
				xReadEpExGolomb( pCabac, &uiSymbol, 1, pBS );
				uiHorAbs += uiSymbol;
			}
			decodeBinEP( pCabac, &uiHorSign, pBS );
		}

		if( bVerAbsGr0 ) {
			if( 2 == uiVerAbs ) {
				xReadEpExGolomb( pCabac, &uiSymbol, 1, pBS );
				uiVerAbs += uiSymbol;
			}
			decodeBinEP( pCabac, &uiVerSign, pBS );
		}

	}
	mvOut->x = uiHorSign ? -int32_t( uiHorAbs ) : uiHorAbs;
	mvOut->y = uiVerSign ? -int32_t( uiVerAbs ) : uiVerAbs;

	//const xMV cMv( uiHorSign ? -Int( uiHorAbs ): uiHorAbs, uiVerSign ? -Int( uiVerAbs ) : uiVerAbs );

#if CHEN_TV
	fprintf( fp_tv, "[%6d] R=%04X, mvd=(%d,%d)\n", g_cnt++, RANGE, cMv.getHor(), cMv.getVer() );
	fflush( fp_tv );
#endif

#if OUT_DEC_TV
	fprintf( fp_tv, "[%6d] mvd=(%d,%d)\n", g_cnt++, cMv.getHor(), cMv.getVer() );
	fflush( fp_tv );
#endif

	return;
}

void xDecompressMvd( xCabac *pCabac, xBitStream *pBS, xMV *pMvdBuf, xContext * ph )
{
	int32_t	 uiTerminalBin	= ph->uiTerminalBin;
	uint32_t uiBinValue		= 0;
	int32_t	 iCtxIdx		= ph->iCtxIdx;

	xMV		 tmp_mvd;

	xCabacDecStart( pCabac, pBS );
	for ( int i=0; i<ph->iNumOfInUnit; i++) {
		parseMvd( pCabac, pBS, &tmp_mvd );
		*(pMvdBuf + i) = tmp_mvd;
	}
	xCabacDecodeBinTrm( pCabac, &uiBinValue, iCtxIdx, pBS );
	if ( uiTerminalBin != uiBinValue) 
		printf("Mismatch at Mvd terminal! \n");
}


void xCabacDecode_test( xCabac *pCabac, xBitStream *pBS, uint8_t *aucOutBuf, xContext *ph )
{
	uint8_t		temp	= 0;
	int32_t		iCtxIdx = ph->iCtxIdx;
	uint32_t	uiBinValue = 0;
	uint32_t	uiTerminalBin = ph->uiTerminalBin;

	xCabacDecStart( pCabac, pBS );

	for ( int i=0; i<ph->iNumOfInUnit; i++) {
		temp = 0;
		for ( int j=0; j<8; j++ ) {
			xCabacDecodeBin( pCabac, &uiBinValue, iCtxIdx, pBS );
			temp |= (uiBinValue & 1) << j;
		}
		aucOutBuf[i] = temp; 
		//if ( aucInBuf[i] != aucOutBuf[i]) {
		//	printf("mismatch at %d, enc:%d, dec:%d!\n", i, aucInBuf[i], aucOutBuf[i]);
		//	exit( 0 );
		//}
	}

	xCabacDecodeBinTrm( pCabac, &uiBinValue, iCtxIdx, pBS );
	if ( uiTerminalBin != uiBinValue) 
		printf("Mismatch at terminal! \n");
}

