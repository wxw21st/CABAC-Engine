/*****************************************************************************
 * config.h: define Macros
 *****************************************************************************
 *
 * Authors: Xiangwen Wang
 *          wxw21st@gmail.com.
 * Time: init at 2015-03-25, 
 * Addr: Leitch Lab of University of Waterloo 
 *****************************************************************************/

#ifndef __CONFIG_H__
#define __CONFIG_H__


typedef unsigned char       uint8_t;
typedef   signed char        int8_t;
typedef unsigned short      uint16_t;
typedef          short       int16_t;

typedef uint8_t pixel;

#ifndef ARCH_X64
typedef unsigned int        uint32_t;
typedef          int         int32_t;
#else
typedef unsigned long       uint32_t;
typedef          long        int32_t;
#endif

#ifdef _MSC_VER
typedef unsigned __int64    uint64_t;
typedef          __int64     int64_t;
#else
typedef unsigned long long  uint64_t;
typedef          long long   int64_t;

typedef int intptr_t;
//from linits.h
#define UINT_MAX      0xffffffff    /* maximum unsigned int value */
#define INT_MAX       2147483647    /* maximum (signed) int value */
#endif

// Fast integer depends on platform
typedef int32_t    int32_t;

typedef const  int8_t      cint8_t;
typedef const uint8_t     cuint8_t;
typedef const  int16_t     cint16_t;
typedef const uint16_t    cuint16_t;
typedef const  int32_t     cint32_t;
typedef const uint32_t    cuint32_t;
typedef const  int64_t     cint64_t;
typedef const uint64_t    cuint64_t;
typedef const  int32_t       cint32_t;
typedef const uint32_t      CUInt;

/// supported slice type
typedef enum {
	SLICE_B = 0,
	SLICE_P = 1,
	SLICE_I = 2,
	X265_MAX_SLICE_TYPE = 3
} xSliceType;

#define TEST_BYTE_SIZE	30
#define MAX_MV_NUM			10000
#define MAX_BUF_SIZE		10000

#endif 