/*****************************************************************************
 * utils.h: little functions
 *****************************************************************************
 *
 * Authors: Xiangwen Wang
 *          wxw21st@gmail.com.
 * Time: Init at 2015-03-25, 
 * Addr: Leitch Lab of University of Waterloo 
 *****************************************************************************/

#ifndef __UTILS_H__
#define __UTILS_H__

#define Clip(x)         Clip3( 0, 255, (x))
#define xSHR(x, n)      ( (n)>=32 ? 0 : ((x)>>(n)) )
#define xSHL(x, n)      ( (n)>=32 ? 0 : ((x)<<(n)) )
#define MAX(a, b)       ( (a) > (b) ? (a) : (b) )
#define MIN(a, b)       ( (a) < (b) ? (a) : (b) )

static int xLog2(uint32_t x)
{
	unsigned long log2Size = 0;
	while(x > 0) {
		x >>= 1;
		log2Size++;
	}
	return(log2Size);
}


static int32_t av_clip_c(int a, int amin, int amax)
{
	if      (a < amin) return amin;
	else if (a > amax) return amax;
	else               return a;
}

static int32_t Clip3( int32_t minVal, int32_t maxVal, int32_t a )
{
	if ( a < minVal )
		a = minVal;
	else if ( a > maxVal )
		a = maxVal;
	return a;
}

static inline int32_t av_clip(int a, int amin, int amax)
{
	return Clip3(amin, amax, a);
}

#endif