/*****************************************************************************
 * main.cpp: main functions: init, set values, enc, dec and check
 *****************************************************************************
 *
 * Authors: Xiangwen Wang
 *          wxw21st@gmail.com.
 * Time: init at 2015-03-25, 
 * Addr: Leitch Lab of University of Waterloo 
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "cabac.h"
#include "bitstream.h"


uint8_t aucBitStreamBuf[MAX_BUF_SIZE+4];
uint8_t aucInBuf[TEST_BYTE_SIZE+4];
uint8_t aucOutBuf[TEST_BYTE_SIZE+4];

xMV		pMvdInBuf[MAX_MV_NUM];
xMV		pMvdOutBuf[MAX_MV_NUM];

void xInitContext( xContext *ph )
{
	ph->eSliceType		= SLICE_P;
	ph->iQP				= 22;
	ph->uiTerminalBin	= 1;
	ph->iCtxIdx			= 3;
}

void xInBitsGenerate( uint8_t *pucInBuf, int iBufSize)
{
	for ( int i=0; i<iBufSize; i++)	{
		*(pucInBuf + i) = (uint8_t) rand();//0xaa;//0xfc;//0xff;//i;//
	}
	return;
}
void xInMvGenerate( xMV *pMvdInBuf, xContext * ph, FILE * fp_in)
{
	printf("input sequence:\n");
	//FILE *fp_in = fopen("d:\\data.txt", "r");
	int temp;
	int i = 0;
	int Eof =0;
	while (!Eof)
	//for ( int i=0; i<iNumMvs; i++)	
	{
		//pMvdInBuf[i].x = 10-i;//0xff;
		if (fscanf(fp_in, "%1d", &temp) != 1)//0xff;
		{
			Eof = 1;
			break;
		}
		pMvdInBuf[i].x = temp;
		printf("%d,", pMvdInBuf[i].x);
		//pMvdInBuf[i].y = 10+i;//0xff;
		if (fscanf(fp_in, "%1d", &temp)!= 1)
		{
			Eof = 1;
			break;
		}
		pMvdInBuf[i].y = temp;
		printf("%d,", pMvdInBuf[i].y);
		//*(pMvInBuf + i) = 0xfc;//0xff;//0xaa;//i;//(uint8_t) rand();//
		i ++;
	}
	ph->iNumOfInUnit = i-2;
	printf("\n");
	//fclose(fp_in);
	return;
}

void xInitCabacAndBs( xCabac * pCabac, xBitStream * pBS, uint8_t * aucBitStreamBuf, xContext *ph )
{
	// set out-buffer to save compressed bit stream and init pBS
	xBitStreamInit( pBS, aucBitStreamBuf, ph->iNumOfInUnit );	
	// set initial state for each pCabac context, according to slice and QP
	xCabacInit(  pCabac, ph->eSliceType, ph->iQP );		
	// int pCabac structure
	xCabacReset( pCabac );									
}

void twoMaxValueInSequence(int32_t *maxValue, int32_t* maxValue_second, int32_t * seq, int32_t num)
{
	int tmpMax = seq[0];
	int tmpIdx = 0, maxValueIdx = 0;
	for (int i=1; i<num; i++)
	{
		if ( tmpMax < seq[i])
		{
			tmpMax = seq[i];
			tmpIdx = i;
		}
	}
	*maxValue = tmpMax;
	maxValueIdx = tmpIdx;

	tmpIdx = tmpIdx<3? 4:2;
	tmpMax = seq[tmpIdx];
	for (int i=0; i<num; i++)
	{
		if ( tmpMax < seq[i] && i != maxValueIdx)
		{
			tmpMax = seq[i];
		}
	}
	*maxValue_second = tmpMax;
}


void fcn(double *tcmp1, double*tcmp2, double *tcmp3, double* tcmp4, double *t1out, int n, 
	double a, double b, double c, double ud, double Ts )
{
	double t1 =0.0, t2 = 0.0, t3 = 0;
	double ta =0.0, tb = 0.0, tc = 0.0, td = 0.0;
	double tx1 = 0.0, tx2 = 0.0, tx3 = 0.0, tx4 = 0.0;
	int N = 0;
	*t1out = 0.0;

	double A, B, C, D, E, F;
	A = 1.732*b*Ts/ud;
	B = (3*a-1.732*b)*Ts/(2*ud);
	C = (a+1.732*b-2*c)*Ts/(2*ud);
	D = (a-1.732*b-2*c)*Ts/(2*ud);
	E = (a+c)*Ts/ud;
	F = (3*a+1.732*b)*Ts/(2*ud);

	switch ( n )
	{
	case 1:
		if (E>=0&&D>=0&&A>=0)
		{N=1;t1=E;t2=D;t3=A;}
		else if(B>=0&&D<=0&&C>=0)
		{N=2;t1=B;t2=-D;t3=C;}
		else if(E<=0&&B>=0&&A>=0)
		{N=13;t1=-E;t2=B;t3=A;}
		else if(B>=0&&A>=0&&C<=0)
		{N=14;t1=B;t2=A;t3=-C;}

		break;
	case 2:
		if (B<=0&&E>=0&&C>=0)
		{N=3;t1=-B;t2=E;t3=C;}
		else if(D<=0&&E<=0&&F>=0)
		{N=4;t1=-D;t2=-E;t3=F;}
		else if(D>=0&&B<=0&&F>=0)
		{N=15;t1=D;t2=-B;t3=F;}
		else if(B<=0&&F>=0&&C<=0)
		{N=16;t1=-B;t2=F;t3=-C;}
		break;

	case 3:
		if (D<=0&&C>=0&&F<=0)
		{N=5;t1=-D;t2=C;t3=-F;}
		else if(A>=0&&C<=0&&E<=0)
		{N=6;t1=A;t2=-C;t3=-E;}
		else if(D>=0&&A>=0&&F<=0)
		{N=17;t1=D;t2=A;t3=-F;}
		else if(A>=0&&F<=0&&E>=0)
		{N=18;t1=A;t2=-F;t3=E;}
		break;
	case 4:
		if (A<=0&&D<=0&&E<=0)
		{N=7;t1=-A;t2=-D;t3=-E;}
		else if(C<=0&&D>=0&&B<=0)
		{N=8;t1=-C;t2=D;t3=-B;}
		else if(C>=0&&A<=0&&B<=0)
		{N=19;t1=C;t2=-A;t3=-B;}
		else if(A<=0&&B<=0&&E>=0)
		{N=20;t1=-A;t2=-B;t3=E;}
		break;

	case 5:
		if (C<=0&&E<=0&&B>=0)
		{N=9;t1=-C;t2=-E;t3=B;}
		else if(F<=0&&E>=0&&D>=0)
		{N=10;t1=-F;t2=E;t3=D;}
		else if(C>=0&&F<=0&&B>=0)
		{N=21;t1=C;t2=-F;t3=B;}
		else if(F<=0&&B>=0&&D<=0)
		{N=22;t1=-F;t2=B;t3=-D;}
		break;
	case 6:
		if (F>=0&&C<=0&&D>=0)
		{N=11;t1=F;t2=-C;t3=D;}
		else if(E>=0&&C>=0&&A<=0)
		{N=12;t1=E;t2=C;t3=-A;}
		else if(E<=0&&F>=0&&A<=0)
		{N=23;t1=-E;t2=F;t3=-A;}
		else if(F>=0&&A<=0&&D<=0)
		{N=24;t1=F;t2=-A;t3=-D;}
		break;
	default:
		break;
	}
	ta=(Ts-t1-t2-t3)/4;
	tb=ta+t1/2;
	tc=tb+t2/2; 
	td=tc+t3/2;
	*t1out=t1;

	switch ( N )
	{
	case 1:
		tx1=ta;tx2=tc;tx3=td;tx4=tb;
		break;
	case 2:
		tx1=ta;tx2=tb;tx3=td;tx4=tc;
		break;
	case 3:
		tx1=tb;tx2=ta;tx3=td;tx4=tc;
		break;
	case 4:
		tx1=tc;tx2=ta;tx3=td;tx4=tb;
		break;
	case 5:
		tx1=td;tx2=ta;tx3=tc;tx4=tb;
		break;
	case 6:
		tx1=td;tx2=ta;tx3=tb;tx4=tc;
		break;
	case 7:
		tx1=td;tx2=tb;tx3=ta;tx4=tc;
		break;
	case 8:
		tx1=td;tx2=tc;tx3=ta;tx4=tb;
		break;
	case 9:
		tx1=tc;tx2=td;tx3=ta;tx4=tb;
		break;
	case 10:
		tx1=tb;tx2=td;tx3=ta;tx4=tc;
		break;
	case 11:
		tx1=ta;tx2=td;tx3=tb;tx4=tc;
		break;
	case 12:
		tx1=ta;tx2=td;tx3=tc;tx4=tb;
		break;
	case 13:
		tx1=tb;tx2=tc;tx3=td;tx4=ta;
		break;
	case 14:
		tx1=ta;tx2=tb;tx3=tc;tx4=td;
		break;
	case 15:
		tx1=tc;tx2=tb;tx3=td;tx4=ta;
		break;
	case 16:
		tx1=tb;tx2=ta;tx3=tc;tx4=td;
		break;
	case 17:
		tx1=td;tx2=tb;tx3=tc;tx4=ta;
		break;
	case 18:
		tx1=tc;tx2=ta;tx3=tb;tx4=td;
		break;
	case 19:
		tx1=td;tx2=tc;tx3=tb;tx4=ta;
		break;
	case 20:
		tx1=tc;tx2=tb;tx3=ta;tx4=td;
		break;
	case 21:
		tx1=tc;tx2=td;tx3=tb;tx4=ta;
		break;
	case 22:
		tx1=tb;tx2=tc;tx3=ta;tx4=td;
		break;
	case 23:
		tx1=tb;tx2=td;tx3=tc;tx4=ta;
		break;
	case 24:
		tx1=ta;tx2=tc;tx3=tb;tx4=td;
		break;
	default:
		break;
	}

	*tcmp1=tx1;
	*tcmp2=tx2;
	*tcmp3=tx3;
	*tcmp4=tx4;
}


void main( int argc, char *argv[])
{
	cint32_t	 iCtxIdx	= 0; // select one context manually
	int32_t		 bAllCorrect= 1;

	xCabac		 cCabac;
	xCabac		*pCabac		= &cCabac;
	xBitStream	 cBS;
	xBitStream	*pBS		= &cBS;

	xContext	 h;
	xContext	*ph = &h;

#if 1
#define NUM			6
	int32_t		seq[NUM] = {0,1,2,-2,-3, -1};
	int32_t		maxValue;
	int32_t		maxValue_second;
	twoMaxValueInSequence(&maxValue, &maxValue_second, seq, NUM);
#endif


	// Set the context index, and set init state according to SliceType and iQP
	xInitContext( ph );


/*****************************codec test**************************************/
#if 1
	ph->iNumOfInUnit	= TEST_BYTE_SIZE;
	// set the input bit stream....
	xInBitsGenerate( aucInBuf, ph->iNumOfInUnit );
	
	// Init Cabac and pBS according to the context h
	xInitCabacAndBs( pCabac, pBS, aucBitStreamBuf, ph );

	// Encode here....
	xCabacEncode_test( pCabac, pBS, aucInBuf, ph ); 

	printf("encoder test finished! InBitsNum: %d, Out  %d of Bits\n", ph->iNumOfInUnit * 8, pBS->iNumOfStreamBytes*8);

	xInitCabacAndBs( pCabac, pBS, aucBitStreamBuf, ph );
	// Decode here....
	xCabacDecode_test( pCabac, pBS, aucOutBuf, ph );

	// Check between input and output here....
	for ( int i=0; i<ph->iNumOfInUnit; i++) {
		if ( aucInBuf[i] != aucOutBuf[i]) {
			printf("mismatch at %d, enc:%d, dec:%d!\n", i, aucInBuf[i], aucOutBuf[i]);
			bAllCorrect = 0;
		}
	}
	if ( bAllCorrect )
		printf( "Congratulation, test part all correct!\n");
	else
		printf( "There is error for test part!\n");

#endif


/*****************************codec Mvd**************************************/
	FILE *fp_in = fopen("d:\\data.txt", "r");

	// Generate input Mvd
	ph->iNumOfInUnit = 100;//MAX_MV_NUM;
	xInMvGenerate( pMvdInBuf, ph, fp_in); 
	fclose(fp_in);

	// Init cabac and BS
	xInitCabacAndBs( pCabac, pBS, aucBitStreamBuf, ph );

	xCompressMvd( pCabac, pBS, pMvdInBuf, ph );

	printf("encoder Mvd finished! InBitsNum: %d, Out  %d of Bits\n", ph->iNumOfInUnit * sizeof(xMV) * 8, pBS->iNumOfStreamBytes*8);

	xInitCabacAndBs( pCabac, pBS, aucBitStreamBuf, ph );

	
	xDecompressMvd( pCabac, pBS, pMvdOutBuf, ph );

	// Check the results
	bAllCorrect = 1;
	for ( int i=0; i<ph->iNumOfInUnit; i++) {
		if (( pMvdInBuf[i].x != pMvdOutBuf[i].x) && ( pMvdInBuf[i].y != pMvdOutBuf[i].y) ) {
			printf("mismatch at %d, inMv (%d:%d), outMv (%d:%d)!\n", 
								i, pMvdInBuf[i].x, pMvdInBuf[i].y, pMvdOutBuf[i].x, pMvdOutBuf[i].y);
			bAllCorrect = 0;
		}
	}

	if ( bAllCorrect )
		printf( "Congratulation, Mvd codec all correct!\n");
	else
		printf( "There is error for Mvd codec!\n");
	system("PAUSE");

}