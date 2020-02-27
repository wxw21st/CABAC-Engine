/*****************************************************************************
 * table.h: tables.cpp
 *****************************************************************************
 *
 * Authors: Xiangwen Wang
 *          wxw21st@gmail.com.
 * Time: Init at 2015-03-25, 
 * Addr: Leitch Lab of University of Waterloo 
 *****************************************************************************/

#ifndef __TABLE_H__
#define __TABLE_H__

// ***************************************************************************
// Cabac Table
// ***************************************************************************
#define MAX_NUM_CTX_MOD             256       ///< maximum number of supported contexts

#define NUM_SPLIT_FLAG_CTX            3       ///< number of context models for split flag
#define NUM_SKIP_FLAG_CTX             3       ///< number of context models for skip flag

#define NUM_MERGE_FLAG_EXT_CTX        1       ///< number of context models for merge flag of merge extended
#define NUM_MERGE_IDX_EXT_CTX         1       ///< number of context models for merge index of merge extended

#define NUM_PART_SIZE_CTX             4       ///< number of context models for partition size
#define NUM_CU_AMP_CTX                1       ///< number of context models for partition size (AMP)
#define NUM_PRED_MODE_CTX             1       ///< number of context models for prediction mode

#define NUM_ADI_CTX                   1       ///< number of context models for intra prediction

#define NUM_CHROMA_PRED_CTX           2       ///< number of context models for intra prediction (chroma)
#define NUM_INTER_DIR_CTX             5//4       ///< number of context models for inter prediction direction
#define NUM_MV_RES_CTX                2       ///< number of context models for motion vector difference

#define NUM_REF_NO_CTX                2       ///< number of context models for reference index
#define NUM_TRANS_SUBDIV_FLAG_CTX     3       ///< number of context models for transform subdivision flags
#define NUM_QT_CBF_CTX                5       ///< number of context models for QT CBF
#define NUM_QT_ROOT_CBF_CTX           1       ///< number of context models for QT ROOT CBF
#define NUM_DELTA_QP_CTX              3       ///< number of context models for dQP

#define NUM_SIG_CG_FLAG_CTX           2       ///< number of context models for MULTI_LEVEL_SIGNIFICANCE

#define NUM_SIG_FLAG_CTX              42      ///< number of context models for sig flag
#define NUM_SIG_FLAG_CTX_LUMA         27      ///< number of context models for luma sig flag
#define NUM_SIG_FLAG_CTX_CHROMA       15      ///< number of context models for chroma sig flag

#define NUM_LAST_FLAG_XY_CTX          15      ///< number of context models for last coefficient position

#define NUM_ONE_FLAG_CTX              24      ///< number of context models for greater than 1 flag
#define NUM_ONE_FLAG_CTX_LUMA         16      ///< number of context models for greater than 1 flag of luma
#define NUM_ONE_FLAG_CTX_CHROMA        8      ///< number of context models for greater than 1 flag of chroma
#define NUM_ABS_FLAG_CTX               6      ///< number of context models for greater than 2 flag
#define NUM_ABS_FLAG_CTX_LUMA          4      ///< number of context models for greater than 2 flag of luma
#define NUM_ABS_FLAG_CTX_CHROMA        2      ///< number of context models for greater than 2 flag of chroma

#define NUM_MVP_IDX_CTX                2      ///< number of context models for MVP index
#define NUM_TRANSFORMSKIP_FLAG_CTX     1      ///< number of context models for transform skipping

#define NUM_SAO_MERGE_FLAG_CTX         1      ///< number of context models for SAO merge flags
#define NUM_SAO_TYPE_IDX_CTX           1      ///< number of context models for SAO type index

#define CNU                          154      ///< dummy initialization value for unused context models 'Context model Not Used'


#define OFF_SPLIT_FLAG_CTX          ( 0 )
#define OFF_SKIP_FLAG_CTX           ( OFF_SPLIT_FLAG_CTX        +   NUM_SPLIT_FLAG_CTX      ) //0+3=3
#define OFF_MERGE_FLAG_EXT_CTX      ( OFF_SKIP_FLAG_CTX         +   NUM_SKIP_FLAG_CTX   )     //3+3=6
#define OFF_MERGE_IDX_EXT_CTX       ( OFF_MERGE_FLAG_EXT_CTX    +   NUM_MERGE_FLAG_EXT_CTX  ) //6+1=7
#define OFF_PART_SIZE_CTX           ( OFF_MERGE_IDX_EXT_CTX     +   NUM_MERGE_IDX_EXT_CTX   ) //7+1=8
#define OFF_CU_AMP_CTX              ( OFF_PART_SIZE_CTX         +   NUM_PART_SIZE_CTX       ) //8+4=12
#define OFF_PRED_MODE_CTX           ( OFF_CU_AMP_CTX            +   NUM_CU_AMP_CTX          ) //12+1=13
#define OFF_INTRA_PRED_CTX          ( OFF_PRED_MODE_CTX         +   NUM_PRED_MODE_CTX       ) //13+1=14
#define OFF_CHROMA_PRED_CTX         ( OFF_INTRA_PRED_CTX        +   NUM_ADI_CTX             ) //14+1=15
#define OFF_INTER_DIR_CTX           ( OFF_CHROMA_PRED_CTX       +   NUM_CHROMA_PRED_CTX     ) //15+2=17
#define OFF_MVD_CTX                 ( OFF_INTER_DIR_CTX         +   NUM_INTER_DIR_CTX       ) //17+5=22 //--4
#define OFF_REF_PIC_CTX             ( OFF_MVD_CTX               +   NUM_MV_RES_CTX          ) //22+2=24
#define OFF_DELTA_QP_CTX            ( OFF_REF_PIC_CTX           +   NUM_REF_NO_CTX          ) //24+2=26
#define OFF_QT_CBF_CTX              ( OFF_DELTA_QP_CTX          +   NUM_DELTA_QP_CTX        ) //26+3=29
#define OFF_QT_ROOT_CBF_CTX         ( OFF_QT_CBF_CTX            + 2*NUM_QT_CBF_CTX          ) //29+10=39
#define OFF_SIG_CG_FLAG_CTX         ( OFF_QT_ROOT_CBF_CTX       +   NUM_QT_ROOT_CBF_CTX     ) //39+1 =40
#define OFF_SIG_FLAG_CTX            ( OFF_SIG_CG_FLAG_CTX       + 2*NUM_SIG_CG_FLAG_CTX     ) //40+4 =44
#define OFF_LAST_X_CTX              ( OFF_SIG_FLAG_CTX          +   NUM_SIG_FLAG_CTX        ) //44+42=86
#define OFF_LAST_Y_CTX              ( OFF_LAST_X_CTX            + 2*NUM_LAST_FLAG_XY_CTX    ) //86+30=116
#define OFF_ONE_FLAG_CTX            ( OFF_LAST_Y_CTX            + 2*NUM_LAST_FLAG_XY_CTX    ) //116+30=146
#define OFF_ABS_FLAG_CTX            ( OFF_ONE_FLAG_CTX          +   NUM_ONE_FLAG_CTX        ) //146+24=170
#define OFF_MVP_IDX_CTX             ( OFF_ABS_FLAG_CTX          +   NUM_ABS_FLAG_CTX        ) //170+6 =176
#define OFF_TRANS_SUBDIV_FLAG_CTX   ( OFF_MVP_IDX_CTX           +   NUM_MVP_IDX_CTX         ) //176+2 =178
#define OFF_TS_FLAG_CTX             ( OFF_TRANS_SUBDIV_FLAG_CTX +   NUM_TRANS_SUBDIV_FLAG_CTX)//178+3 =181
#define OFF_SAO_MERGE_FLAG_CTX      ( OFF_TS_FLAG_CTX           +   NUM_TRANSFORMSKIP_FLAG_CTX)//181+1 =182
#define OFF_SAO_TYPE_IDX_CTX        ( OFF_SAO_MERGE_FLAG_CTX    +   NUM_SAO_MERGE_FLAG_CTX)   //182+1 =183

// ***************************************************************************
//uint8_t	  g_nextState[128][2];
const uint8_t g_aucNextStateMPS[128] =
{
	2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
	18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
	34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
	50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
	66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
	82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
	98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
	114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 124, 125, 126, 127
};

const uint8_t g_aucNextStateLPS[128] =
{
	1, 0, 0, 1, 2, 3, 4, 5, 4, 5, 8, 9, 8, 9, 10, 11,
	12, 13, 14, 15, 16, 17, 18, 19, 18, 19, 22, 23, 22, 23, 24, 25,
	26, 27, 26, 27, 30, 31, 30, 31, 32, 33, 32, 33, 36, 37, 36, 37,
	38, 39, 38, 39, 42, 43, 42, 43, 44, 45, 44, 45, 46, 47, 48, 49,
	48, 49, 50, 51, 52, 53, 52, 53, 54, 55, 54, 55, 56, 57, 58, 59,
	58, 59, 60, 61, 60, 61, 60, 61, 62, 63, 64, 65, 64, 65, 66, 67,
	66, 67, 66, 67, 68, 69, 68, 69, 70, 71, 70, 71, 70, 71, 72, 73,
	72, 73, 72, 73, 74, 75, 74, 75, 74, 75, 76, 77, 76, 77, 126, 127
};


/*
* g_nextState[128][2] is built from xBuildNextStateTable (attached below)
* Originally, this table is built dynamically every time an encoder is initialized
* I found it is constant during the encoding and is shared by all threads, 
* so i make it a const table by off-line calculating
*
*/
const uint8_t g_nextState[128][2] = {
	2 ,   1 , 
	0 ,   3 , 
	4 ,   0 , 
	1 ,   5 , 
	6 ,   2 , 
	3 ,   7 , 
	8 ,   4 , 
	5 ,   9 , 
	10 ,   4 , 
	5 ,  11 , 
	12 ,   8 , 
	9 ,  13 , 
	14 ,   8 , 
	9 ,  15 , 
	16 ,  10 , 
	11 ,  17 , 
	18 ,  12 , 
	13 ,  19 , 
	20 ,  14 , 
	15 ,  21 , 
	22 ,  16 , 
	17 ,  23 , 
	24 ,  18 , 
	19 ,  25 , 
	26 ,  18 , 
	19 ,  27 , 
	28 ,  22 , 
	23 ,  29 , 
	30 ,  22 , 
	23 ,  31 , 
	32 ,  24 , 
	25 ,  33 , 
	34 ,  26 , 
	27 ,  35 , 
	36 ,  26 , 
	27 ,  37 , 
	38 ,  30 , 
	31 ,  39 , 
	40 ,  30 , 
	31 ,  41 , 
	42 ,  32 , 
	33 ,  43 , 
	44 ,  32 , 
	33 ,  45 , 
	46 ,  36 , 
	37 ,  47 , 
	48 ,  36 , 
	37 ,  49 , 
	50 ,  38 , 
	39 ,  51 , 
	52 ,  38 , 
	39 ,  53 , 
	54 ,  42 , 
	43 ,  55 , 
	56 ,  42 , 
	43 ,  57 , 
	58 ,  44 , 
	45 ,  59 , 
	60 ,  44 , 
	45 ,  61 , 
	62 ,  46 , 
	47 ,  63 , 
	64 ,  48 , 
	49 ,  65 , 
	66 ,  48 , 
	49 ,  67 , 
	68 ,  50 , 
	51 ,  69 , 
	70 ,  52 , 
	53 ,  71 , 
	72 ,  52 , 
	53 ,  73 , 
	74 ,  54 , 
	55 ,  75 , 
	76 ,  54 , 
	55 ,  77 , 
	78 ,  56 , 
	57 ,  79 , 
	80 ,  58 , 
	59 ,  81 , 
	82 ,  58 , 
	59 ,  83 , 
	84 ,  60 , 
	61 ,  85 , 
	86 ,  60 , 
	61 ,  87 , 
	88 ,  60 , 
	61 ,  89 , 
	90 ,  62 , 
	63 ,  91 , 
	92 ,  64 , 
	65 ,  93 , 
	94 ,  64 , 
	65 ,  95 , 
	96 ,  66 , 
	67 ,  97 , 
	98 ,  66 , 
	67 ,  99 , 
	100 ,  66 , 
	67 , 101 , 
	102 ,  68 , 
	69 , 103 , 
	104 ,  68 , 
	69 , 105 , 
	106 ,  70 , 
	71 , 107 , 
	108 ,  70 , 
	71 , 109 , 
	110 ,  70 , 
	71 , 111 , 
	112 ,  72 , 
	73 , 113 , 
	114 ,  72 , 
	73 , 115 , 
	116 ,  72 , 
	73 , 117 , 
	118 ,  74 , 
	75 , 119 , 
	120 ,  74 , 
	75 , 121 , 
	122 ,  74 , 
	75 , 123 , 
	124 ,  76 , 
	77 , 125 , 
	124 ,  76 , 
	77 , 125 , 
	126 , 126 , 
	127 , 127 , 
};




// Table 9-43 ¨C State transition table
cuint8_t xg_aucNextStateMPS[128] = {
	2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
	18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
	34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
	50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
	66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
	82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
	98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
	114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 124, 125, 126, 127
};
cuint8_t xg_aucNextStateLPS[128] = {
	1, 0, 0, 1, 2, 3, 4, 5, 4, 5, 8, 9, 8, 9, 10, 11,
	12, 13, 14, 15, 16, 17, 18, 19, 18, 19, 22, 23, 22, 23, 24, 25,
	26, 27, 26, 27, 30, 31, 30, 31, 32, 33, 32, 33, 36, 37, 36, 37,
	38, 39, 38, 39, 42, 43, 42, 43, 44, 45, 44, 45, 46, 47, 48, 49,
	48, 49, 50, 51, 52, 53, 52, 53, 54, 55, 54, 55, 56, 57, 58, 59,
	58, 59, 60, 61, 60, 61, 60, 61, 62, 63, 64, 65, 64, 65, 66, 67,
	66, 67, 66, 67, 68, 69, 68, 69, 70, 71, 70, 71, 70, 71, 72, 73,
	72, 73, 72, 73, 74, 75, 74, 75, 74, 75, 76, 77, 76, 77, 126, 127
};

// Table 9-42 ¨C Specification of rangeTabLPS depending on pStateIdx and qCodIRangeIdx
cuint8_t xg_aucLPSTable[64][4] = {
	{ 128, 176, 208, 240},
	{ 128, 167, 197, 227},
	{ 128, 158, 187, 216},
	{ 123, 150, 178, 205},
	{ 116, 142, 169, 195},
	{ 111, 135, 160, 185},
	{ 105, 128, 152, 175},
	{ 100, 122, 144, 166},
	{  95, 116, 137, 158},
	{  90, 110, 130, 150},
	{  85, 104, 123, 142},
	{  81,  99, 117, 135},
	{  77,  94, 111, 128},
	{  73,  89, 105, 122},
	{  69,  85, 100, 116},
	{  66,  80,  95, 110},
	{  62,  76,  90, 104},
	{  59,  72,  86,  99},
	{  56,  69,  81,  94},
	{  53,  65,  77,  89},
	{  51,  62,  73,  85},
	{  48,  59,  69,  80},
	{  46,  56,  66,  76},
	{  43,  53,  63,  72},
	{  41,  50,  59,  69},
	{  39,  48,  56,  65},
	{  37,  45,  54,  62},
	{  35,  43,  51,  59},
	{  33,  41,  48,  56},
	{  32,  39,  46,  53},
	{  30,  37,  43,  50},
	{  29,  35,  41,  48},
	{  27,  33,  39,  45},
	{  26,  31,  37,  43},
	{  24,  30,  35,  41},
	{  23,  28,  33,  39},
	{  22,  27,  32,  37},
	{  21,  26,  30,  35},
	{  20,  24,  29,  33},
	{  19,  23,  27,  31},
	{  18,  22,  26,  30},
	{  17,  21,  25,  28},
	{  16,  20,  23,  27},
	{  15,  19,  22,  25},
	{  14,  18,  21,  24},
	{  14,  17,  20,  23},
	{  13,  16,  19,  22},
	{  12,  15,  18,  21},
	{  12,  14,  17,  20},
	{  11,  14,  16,  19},
	{  11,  13,  15,  18},
	{  10,  12,  15,  17},
	{  10,  12,  14,  16},
	{   9,  11,  13,  15},
	{   9,  11,  12,  14},
	{   8,  10,  12,  14},
	{   8,   9,  11,  13},
	{   7,   9,  11,  12},
	{   7,   9,  10,  12},
	{   7,   8,  10,  11},
	{   6,   8,   9,  11},
	{   6,   7,   9,  10},
	{   6,   7,   8,   9},
	{   2,   2,   2,   2}
};

cuint8_t xg_aucRenormTable[32] = {
	6,  5,  4,  4,
	3,  3,  3,  3,
	2,  2,  2,  2,
	2,  2,  2,  2,
	1,  1,  1,  1,
	1,  1,  1,  1,
	1,  1,  1,  1,
	1,  1,  1,  1
};



// ***************************************************************************
// * Cabac tables
// ***************************************************************************
cuint8_t
	INIT_SPLIT_FLAG[3][NUM_SPLIT_FLAG_CTX] = {
		{ 107,  139,  126, },
		{ 107,  139,  126, },
		{ 139,  141,  157, },
};

cuint8_t
	INIT_SKIP_FLAG[3][NUM_SKIP_FLAG_CTX] = {
		{ 197,  185,  201, },
		{ 197,  185,  201, },
		{ CNU,  CNU,  CNU, },
};

cuint8_t
	INIT_MERGE_FLAG_EXT[3][NUM_MERGE_FLAG_EXT_CTX] = {
		{ 154, },
		{ 110, },
		{ CNU, },
};

cuint8_t
	INIT_MERGE_IDX_EXT[3][NUM_MERGE_IDX_EXT_CTX] = {
		{ 137, },
		{ 122, },
		{ CNU, },
};

cuint8_t
	INIT_PART_SIZE[3][NUM_PART_SIZE_CTX] = {
		{ 154,  139,  CNU,  CNU, },
		{ 154,  139,  CNU,  CNU, },
		{ 184,  CNU,  CNU,  CNU, },
};

cuint8_t
	INIT_CU_AMP_POS[3][NUM_CU_AMP_CTX] = {
		{ 154, },
		{ 154, },
		{ CNU, },
};

cuint8_t
	INIT_PRED_MODE[3][NUM_PRED_MODE_CTX] = {
		{ 134, },
		{ 149, },
		{ CNU, },
};

cuint8_t
	INIT_INTRA_PRED_MODE[3][NUM_ADI_CTX] = {
		{ 183, },
		{ 154, },
		{ 184, },
};

cuint8_t
	INIT_CHROMA_PRED_MODE[3][NUM_CHROMA_PRED_CTX] = {
		{ 152,  139, },
		{ 152,  139, },
		{  63,  139, },
};

cuint8_t
	INIT_INTER_DIR[3][NUM_INTER_DIR_CTX] = {
		{  95,   79,   63,   31, 31, },  //add the last 31 by wxw
		{  95,   79,   63,   31, 31, },
		{ CNU,  CNU,  CNU,  CNU, CNU},
};

cuint8_t
	INIT_MVD[3][NUM_MV_RES_CTX] = {
		{ 169,  198, },
		{ 140,  198, },
		{ CNU,  CNU, },
};

cuint8_t
	INIT_REF_PIC[3][NUM_REF_NO_CTX] = {
		{ 153,  153 }, 
		{ 153,  153 }, 
		{ CNU,  CNU }, 
};

cuint8_t
	INIT_DQP[3][NUM_DELTA_QP_CTX] = {
		{ 154,  154,  154, },
		{ 154,  154,  154, },
		{ 154,  154,  154, },
};

cuint8_t
	INIT_QT_CBF[3][2*NUM_QT_CBF_CTX] = {
		{ 153,  111,  CNU,  CNU,  CNU,  149,   92,  167,  CNU,  CNU, },
		{ 153,  111,  CNU,  CNU,  CNU,  149,  107,  167,  CNU,  CNU, },
		{ 111,  141,  CNU,  CNU,  CNU,   94,  138,  182,  CNU,  CNU, },
};

cuint8_t
	INIT_QT_ROOT_CBF[3][NUM_QT_ROOT_CBF_CTX] = {
		{  79, },
		{  79, },
		{ CNU, },
};

cuint8_t
	INIT_LAST[3][2*NUM_LAST_FLAG_XY_CTX] = {
		{
			125,  110,  124,  110,   95,   94,  125,  111,  111,   79,  125,  126,  111,  111,   79,
			108,  123,   93,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,
		},
		{
			125,  110,   94,  110,   95,   79,  125,  111,  110,   78,  110,  111,  111,   95,   94,
			108,  123,  108,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,
		},
		{
			110,  110,  124,  125,  140,  153,  125,  127,  140,  109,  111,  143,  127,  111,   79,
			108,  123,   63,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,  CNU,
		},
};

cuint8_t
	INIT_SIG_CG_FLAG[3][2 * NUM_SIG_CG_FLAG_CTX] = {
		{
			121,  140,
			61,  154,
		},
		{
			121,  140,
			61,  154,
		},
		{
			91,  171,
			134,  141,
		},
};

cuint8_t
	INIT_SIG_FLAG[3][NUM_SIG_FLAG_CTX] = {
		{ 170,  154,  139,  153,  139,  123,  123,   63,  124,  166,  183,  140,  136,  153,  154,  166,  183,  140,  136,  153,  154,  166,  183,  140,  136,  153,  154,  170,  153,  138,  138,  122,  121,  122,  121,  167,  151,  183,  140,  151,  183,  140,  }, 
		{ 155,  154,  139,  153,  139,  123,  123,   63,  153,  166,  183,  140,  136,  153,  154,  166,  183,  140,  136,  153,  154,  166,  183,  140,  136,  153,  154,  170,  153,  123,  123,  107,  121,  107,  121,  167,  151,  183,  140,  151,  183,  140,  }, 
		{ 111,  111,  125,  110,  110,   94,  124,  108,  124,  107,  125,  141,  179,  153,  125,  107,  125,  141,  179,  153,  125,  107,  125,  141,  179,  153,  125,  140,  139,  182,  182,  152,  136,  152,  136,  153,  136,  139,  111,  136,  139,  111,  }, 
};

cuint8_t
	INIT_ONE_FLAG[3][NUM_ONE_FLAG_CTX] = {
		{ 154,  196,  167,  167,  154,  152,  167,  182,  182,  134,  149,  136,  153,  121,  136,  122,  169,  208,  166,  167,  154,  152,  167,  182, },
		{ 154,  196,  196,  167,  154,  152,  167,  182,  182,  134,  149,  136,  153,  121,  136,  137,  169,  194,  166,  167,  154,  167,  137,  182, },
		{ 140,   92,  137,  138,  140,  152,  138,  139,  153,   74,  149,   92,  139,  107,  122,  152,  140,  179,  166,  182,  140,  227,  122,  197, },
};

cuint8_t
	INIT_ABS_FLAG[3][NUM_ABS_FLAG_CTX] = {
		{ 107,  167,   91,  107,  107,  167, },
		{ 107,  167,   91,  122,  107,  167, },
		{ 138,  153,  136,  167,  152,  152, },
};

cuint8_t
	INIT_MVP_IDX[3][NUM_MVP_IDX_CTX] = {
		{ 168,  CNU, },
		{ 168,  CNU, },
		{ CNU,  CNU, },
};

cuint8_t
	INIT_TRANS_SUBDIV_FLAG[3][NUM_TRANS_SUBDIV_FLAG_CTX] = {
		{ 224,  167,  122, },
		{ 124,  138,   94, },
		{ 153,  138,  138, },
};

cuint8_t
	INIT_TRANSFORMSKIP_FLAG[3][2*NUM_TRANSFORMSKIP_FLAG_CTX] = {
		{ 139,  139},
		{ 139,  139},
		{ 139,  139},
};

cuint8_t
	INIT_SAO_MERGE_FLAG[3][NUM_SAO_MERGE_FLAG_CTX] =
{
	{ 153,  },
	{ 153,  },
	{ 153,  },
};

cuint8_t
	INIT_SAO_TYPE_IDX[3][NUM_SAO_TYPE_IDX_CTX] =
{
	{ 160, },
	{ 185, },
	{ 200, },
};
#endif