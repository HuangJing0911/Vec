//
// Coreco API - Buffer functions.
//

#ifndef _CAPIBUF_H_
#define _CAPIBUF_H_

#include <capidef.h>

/****************************************************************************
*	Buffer Capabilities
*
*	Last capability #: 0
****************************************************************************/
#define CORBUFFER_CAP_FIRST			0
#define CORBUFFER_CAP( n, size)		((((n) + CORBUFFER_CAP_FIRST) << 16) | (size))

#define CORBUFFER_CAP_PIXEL_DEPTH    CORBUFFER_CAP( 0, 4)

/****************************************************************************
*	Buffer Parameters
*
*	Last parameter #: 5
****************************************************************************/
#define CORBUFFER_PRM_FIRST			0
#define CORBUFFER_PRM( n, size)		((((n) + CORBUFFER_PRM_FIRST) << 16) | (size))

#define CORBUFFER_PRM_FORMAT			 CORBUFFER_PRM( 0, 4)
#define CORBUFFER_PRM_TYPE 			 CORBUFFER_PRM( 1, 4)
#define CORBUFFER_PRM_DATASIZE		 CORBUFFER_PRM( 2, 4)
#define CORBUFFER_PRM_DATADEPTH		 CORBUFFER_PRM( 3, 4)
#define CORBUFFER_PRM_XMIN				 CORBUFFER_PRM( 4, 4)
#define CORBUFFER_PRM_YMIN				 CORBUFFER_PRM( 5, 4)
#define CORBUFFER_PRM_WIDTH			 CORBUFFER_PRM( 6, 4)
#define CORBUFFER_PRM_HEIGHT			 CORBUFFER_PRM( 7, 4)
#define CORBUFFER_PRM_ADDRESS			 CORBUFFER_PRM( 8, sizeof(void *))
#define CORBUFFER_PRM_PHYSADDRESS	 CORBUFFER_PRM( 9, sizeof(ULONG_PTR))
#define CORBUFFER_PRM_ROOT				 CORBUFFER_PRM(10, sizeof( _CORHANDLE))
#define CORBUFFER_PRM_PARENT			 CORBUFFER_PRM(11, sizeof( _CORHANDLE))
#define CORBUFFER_PRM_MEM_WIDTH		 CORBUFFER_PRM( 12, 4)
#define CORBUFFER_PRM_MEM_HEIGHT		 CORBUFFER_PRM( 13, 4)
#define CORBUFFER_PRM_STATE			 CORBUFFER_PRM( 14, 4)
#define CORBUFFER_PRM_SIGNED			 CORBUFFER_PRM( 15, 4)
#define CORBUFFER_PRM_NPAGES         CORBUFFER_PRM( 16, 4)
#define CORBUFFER_PRM_PAGE           CORBUFFER_PRM( 17, 4)
#define CORBUFFER_PRM_COUNTER_STAMP  CORBUFFER_PRM( 18, 4)
#define CORBUFFER_PRM_SPACE_USED     CORBUFFER_PRM( 19, 4)
#define CORBUFFER_PRM_INBWIN_ADDRESS CORBUFFER_PRM( 20, sizeof(ULONG_PTR))
#define CORBUFFER_PRM_NSEG_USED      CORBUFFER_PRM( 21, 4)
#define CORBUFFER_PRM_SG_LIST        CORBUFFER_PRM( 22, sizeof(PCORSGENTRY *))
#define CORBUFFER_PRM_SG_LIST_PHYSADDRESS   CORBUFFER_PRM( 23, sizeof(PCORSGENTRY *))
#define CORBUFFER_PRM_LOCKED         CORBUFFER_PRM( 24, 4)
#define CORBUFFER_PRM_PITCH         CORBUFFER_PRM( 25, 4)
#define CORBUFFER_PRM_PIXEL_DEPTH    CORBUFFER_PRM( 26, 4)
#define CORBUFFER_PRM_HW_STATE_MASK	 CORBUFFER_PRM( 27, 4)
#define CORBUFFER_PRM_HOST_COUNTER_STAMP  CORBUFFER_PRM( 28, 8)
#define CORBUFFER_PRM_MULTI_INDEX    CORBUFFER_PRM( 29, 4)

// For backward compatility															
#define CORBUFFER_PRM_SPACEUSED     CORBUFFER_PRM_SPACE_USED

/* Buffer Formats */
#define CORBUFFER_VAL_FORMAT_SIGNED		CORDATA_FORMAT_SIGNED
#define CORBUFFER_VAL_FORMAT_UNSIGNED	CORDATA_FORMAT_UNSIGNED

#define CORBUFFER_VAL_FORMAT_MONO1		CORDATA_FORMAT_MONO1
#define CORBUFFER_VAL_FORMAT_UINT1		CORDATA_FORMAT_UINT1
#define CORBUFFER_VAL_FORMAT_BINARY		CORDATA_FORMAT_BINARY

#define CORBUFFER_VAL_FORMAT_MONO8		CORDATA_FORMAT_MONO8
#define CORBUFFER_VAL_FORMAT_RGBP8     CORDATA_FORMAT_RGBP8
#define CORBUFFER_VAL_FORMAT_INT8		CORDATA_FORMAT_INT8
#define CORBUFFER_VAL_FORMAT_UINT8		CORDATA_FORMAT_UINT8

#define CORBUFFER_VAL_FORMAT_MONO16		CORDATA_FORMAT_MONO16
#define CORBUFFER_VAL_FORMAT_RGBP16    CORDATA_FORMAT_RGBP16
#define CORBUFFER_VAL_FORMAT_INT16		CORDATA_FORMAT_INT16
#define CORBUFFER_VAL_FORMAT_UINT16		CORDATA_FORMAT_UINT16

#define CORBUFFER_VAL_FORMAT_MONO24		CORDATA_FORMAT_MONO24
#define CORBUFFER_VAL_FORMAT_INT24		CORDATA_FORMAT_INT24
#define CORBUFFER_VAL_FORMAT_UINT24		CORDATA_FORMAT_UINT24

#define CORBUFFER_VAL_FORMAT_MONO32		CORDATA_FORMAT_MONO32
#define CORBUFFER_VAL_FORMAT_INT32		CORDATA_FORMAT_INT32
#define CORBUFFER_VAL_FORMAT_UINT32		CORDATA_FORMAT_UINT32

#define CORBUFFER_VAL_FORMAT_MONO64		CORDATA_FORMAT_MONO64
#define CORBUFFER_VAL_FORMAT_INT64		CORDATA_FORMAT_INT64
#define CORBUFFER_VAL_FORMAT_UINT64		CORDATA_FORMAT_UINT64

#define CORBUFFER_VAL_FORMAT_RGB5551	CORDATA_FORMAT_RGB5551
#define CORBUFFER_VAL_FORMAT_RGB565		CORDATA_FORMAT_RGB565
#define CORBUFFER_VAL_FORMAT_RGB888		CORDATA_FORMAT_RGB888
#define CORBUFFER_VAL_FORMAT_RGBR888	CORDATA_FORMAT_RGBR888
#define CORBUFFER_VAL_FORMAT_RGB8888	CORDATA_FORMAT_RGB8888
#define CORBUFFER_VAL_FORMAT_RGB101010 CORDATA_FORMAT_RGB101010
#define CORBUFFER_VAL_FORMAT_RGB161616 CORDATA_FORMAT_RGB161616
#define CORBUFFER_VAL_FORMAT_RGB16161616 CORDATA_FORMAT_RGB16161616

#define CORBUFFER_VAL_FORMAT_HSV			CORDATA_FORMAT_HSV
#define CORBUFFER_VAL_FORMAT_HSI			CORDATA_FORMAT_HSI
#define CORBUFFER_VAL_FORMAT_HSIP8		CORDATA_FORMAT_HSIP8
#define CORBUFFER_VAL_FORMAT_LAB			CORDATA_FORMAT_LAB
#define CORBUFFER_VAL_FORMAT_LABP8		CORDATA_FORMAT_LABP8
#define CORBUFFER_VAL_FORMAT_LABP16		CORDATA_FORMAT_LABP16
#define CORBUFFER_VAL_FORMAT_LAB16161616 CORDATA_FORMAT_LAB16161616
#define CORBUFFER_VAL_FORMAT_LAB101010 CORDATA_FORMAT_LAB101010

#define CORBUFFER_VAL_FORMAT_UYVY		CORDATA_FORMAT_UYVY
#define CORBUFFER_VAL_FORMAT_YUY2		CORDATA_FORMAT_YUY2
#define CORBUFFER_VAL_FORMAT_YVYU		CORDATA_FORMAT_YVYU
#define CORBUFFER_VAL_FORMAT_YUYV		CORDATA_FORMAT_YUYV
#define CORBUFFER_VAL_FORMAT_Y411		CORDATA_FORMAT_Y411
#define CORBUFFER_VAL_FORMAT_Y211		CORDATA_FORMAT_Y211
#define CORBUFFER_VAL_FORMAT_YUV			CORDATA_FORMAT_YUV
#define CORBUFFER_VAL_FORMAT_AYUV		CORDATA_FORMAT_AYUV
#define CORBUFFER_VAL_FORMAT_IYU1      CORDATA_FORMAT_IYU1
#define CORBUFFER_VAL_FORMAT_IYU2		CORDATA_FORMAT_IYU2

#define CORBUFFER_VAL_FORMAT_FLOAT		CORDATA_FORMAT_FLOAT
#define CORBUFFER_VAL_FORMAT_COMPLEX	CORDATA_FORMAT_COMPLEX
#define CORBUFFER_VAL_FORMAT_POINT		CORDATA_FORMAT_POINT
#define CORBUFFER_VAL_FORMAT_FPOINT		CORDATA_FORMAT_FPOINT


/* Format Field Access Macros */
#define CORBUFFER_FORMAT_INDEX(format)				CORDATA_FORMAT_INDEX(format)
#define CORBUFFER_FORMAT_DATADEPTH(format)		CORDATA_FORMAT_DATADEPTH(format)
#define CORBUFFER_FORMAT_DATASIZE(format)			CORDATA_FORMAT_DATASIZE(format)
#define CORBUFFER_FORMAT_IS_SIGNED(format)		CORDATA_FORMAT_IS_SIGNED(format)
#define CORBUFFER_FORMAT_NPAGES(format)			CORDATA_FORMAT_NPAGES(format)
#define CORBUFFER_FORMAT_SET_SIGN(format, sign)	CORDATA_FORMAT_SET_SIGN(format, sign)

/* Buffer Types */
#define CORBUFFER_VAL_TYPE_VIRTUAL				0x00000000 
#define CORBUFFER_VAL_TYPE_PRO               CORBUFFER_VAL_TYPE_VIRTUAL  // CORBUFFER_VAL_TYPE_PRO obsolete
#define CORBUFFER_VAL_TYPE_CONTIGUOUS			0x00000001
#define CORBUFFER_VAL_TYPE_VIDEO					0x00000002
#define CORBUFFER_VAL_TYPE_OVERLAY				0x00000004
#define CORBUFFER_VAL_TYPE_OFFSCREEN			0x00000008
#define CORBUFFER_VAL_TYPE_USER					0x00000010
#define CORBUFFER_VAL_TYPE_SCATTER_GATHER		0x00000020
#define CORBUFFER_VAL_TYPE_DUMMY		         0x00000040
#define CORBUFFER_VAL_TYPE_PHYSICAL	         0x00000080
#define CORBUFFER_VAL_TYPE_SHARED				0x00000100
#define CORBUFFER_VAL_TYPE_UNMAPPED	         0x00000200

// macros used when creating a video buffer on a specific video board
#define CORBUFFER_VIDEO_BOARD_INDEX_NBITS  4
#define CORBUFFER_VIDEO_BOARD_INDEX_SHIFT  (32-CORBUFFER_VIDEO_BOARD_INDEX_NBITS)
#define CORBUFFER_VIDEO_BOARD_INDEX_MASK   (0xFFFF<<CORBUFFER_VIDEO_BOARD_INDEX_SHIFT)
#define CORBUFFER_VIDEO_BOARD_INDEX(index) \
         ((index<<CORBUFFER_VIDEO_BOARD_INDEX_SHIFT) & CORBUFFER_VIDEO_BOARD_INDEX_MASK)

/* States */
#define CORBUFFER_VAL_STATE_EMPTY	0x00000000
#define CORBUFFER_VAL_STATE_FULL    0x80000000

#define CORBUFFER_VAL_STATE_GOOD		0x00000000
#define CORBUFFER_VAL_STATE_OVERFLOW    0x40000000

#define CORBUFFER_VAL_STATE_MASK    0xffff0000

#define CORBUFFER_STATE_IS_EMPTY( state)		( ((state) & CORBUFFER_VAL_STATE_FULL) == 0)
#define CORBUFFER_STATE_IS_FULL( state)		( ((state) & CORBUFFER_VAL_STATE_FULL) != 0)
#define CORBUFFER_STATE_GET_TAG( state)		( (state) & ~(CORBUFFER_VAL_STATE_FULL | CORBUFFER_VAL_STATE_BAD))

#define CORBUFFER_STATE_IS_GOOD( state)			( ((state) & CORBUFFER_VAL_STATE_OVERFLOW) == 0)
#define CORBUFFER_STATE_IS_OVERFLOW( state)		( ((state) & CORBUFFER_VAL_STATE_OVERFLOW) != 0)

#define CORBUFFER_VAL_HW_STATE_MASK_USED				0x80000000
#define CORBUFFER_VAL_HW_STATE_MASK_NOT_USED			0x00000000
#define CORBUFFER_VAL_HW_STATE_MASK_POLARITY			0x40000000	//when '0' then empty bit=0, when 0x40000000 then empty bit='1'

#define CORBUFFER_VAL_FLAG_MULTI           0x01  // Buffer was allocated as multibuffer.
#define CORBUFFER_VAL_FLAG_FIRST_OF_MULTI  0x02  // Indicates that the current buffer handle is first of multibuffer

typedef struct
{
	_CORBUFFER parent;
	_CORBUFFER root;
} CORBUFFERLOCALINFO, *PCORBUFFERLOCALINFO;

#define CORBUFFER_LOCAL_INFO( buffer)	((PCORBUFFERLOCALINFO)CORHANDLE_LOCAL_INFO( buffer))

#define CORBUFFER_LOCAL_INFO_PARENT( buffer)	(&CORBUFFER_LOCAL_INFO( buffer)->parent)
#define CORBUFFER_LOCAL_INFO_ROOT( buffer)	(&CORBUFFER_LOCAL_INFO( buffer)->root)

/* File Formats */
#define	CORBUFFER_FILEFORMAT_BMP	CORDATA_FILEFORMAT_BMP
#define	CORBUFFER_FILEFORMAT_TIFF	CORDATA_FILEFORMAT_TIFF
#define	CORBUFFER_FILEFORMAT_CRC	CORDATA_FILEFORMAT_CRC

/* Bayer Filter Specific (alignment) */
#define CORBUFFER_VAL_BAYER_ALIGN_GB_RG   CORBAYER_ALIGN_GB_RG
#define CORBUFFER_VAL_BAYER_ALIGN_BG_GR   CORBAYER_ALIGN_BG_GR
#define CORBUFFER_VAL_BAYER_ALIGN_RG_GB   CORBAYER_ALIGN_RG_GB
#define CORBUFFER_VAL_BAYER_ALIGN_GR_BG   CORBAYER_ALIGN_GR_BG
#define CORBUFFER_VAL_BAYER_ALIGN_MASK    CORBAYER_ALIGN_MASK

/* Bayer Filter Specific (method) */
#define CORBUFFER_VAL_BAYER_METHOD_1	CORBAYER_METHOD_1
#define CORBUFFER_VAL_BAYER_METHOD_2	CORBAYER_METHOD_2
#define CORBUFFER_VAL_BAYER_METHOD_3	CORBAYER_METHOD_3
#define CORBUFFER_VAL_BAYER_METHOD_4	CORBAYER_METHOD_4
#define CORBUFFER_VAL_BAYER_METHOD_5	CORBAYER_METHOD_5
#define CORBUFFER_VAL_BAYER_METHOD_6	CORBAYER_METHOD_6

#define CORBUFFER_VAL_BAYER_METHOD_INTERPOLATE  CORBUFFER_VAL_BAYER_METHOD_1
#define CORBUFFER_VAL_BAYER_METHOD_GOLDEN_EAGLE CORBUFFER_VAL_BAYER_METHOD_2
#define CORBUFFER_VAL_BAYER_METHOD_EAGLE        CORBUFFER_VAL_BAYER_METHOD_3
#define CORBUFFER_VAL_BAYER_METHOD_MASK         CORBAYER_METHOD_MASK

/* Format Conversion Specific (clipping -> bits 0..3) */ 
#define CORBUFFER_VAL_CONVERT_RANGE_CLIP  0x00000001
#define CORBUFFER_VAL_CONVERT_RANGE_REMAP 0x00000002
#define CORBUFFER_VAL_CONVERT_RANGE_MASK  0x0000000F

/* Format Conversion Specific (complex type -> bits 4..7) */ 
#define CORBUFFER_VAL_CONVERT_COMPLEX_MAG_PHASE 0x00000010
#define CORBUFFER_VAL_CONVERT_COMPLEX_REAL_IMAG 0x00000020
#define CORBUFFER_VAL_CONVERT_COMPLEX_MASK      0x000000F0

/* Format Conversion Specific (sign change -> bit 8) */ 
#define CORBUFFER_VAL_CONVERT_CHANGE_SIGN 0x00000100

/* Definitions for backward compatibility */
#define CORBUFFER_BAYER_ALIGN_GB_RG CORBUFFER_VAL_BAYER_ALIGN_GB_RG
#define CORBUFFER_BAYER_ALIGN_BG_GR CORBUFFER_VAL_BAYER_ALIGN_BG_GR
#define CORBUFFER_BAYER_ALIGN_RG_GB CORBUFFER_VAL_BAYER_ALIGN_RG_GB
#define CORBUFFER_BAYER_ALIGN_GR_BG CORBUFFER_VAL_BAYER_ALIGN_GR_BG
#define CORBUFFER_BAYER_ALIGN_MASK  CORBUFFER_VAL_BAYER_ALIGN_MASK

#define CORBUFFER_BAYER_METHOD_INTERPOLATE   CORBUFFER_VAL_BAYER_METHOD_1
#define CORBUFFER_BAYER_METHOD_GOLDEN_EAGLE  CORBUFFER_VAL_BAYER_METHOD_2
#define CORBUFFER_BAYER_METHOD_EAGLE         CORBUFFER_VAL_BAYER_METHOD_3
#define CORBUFFER_BAYER_METHOD_MASK          CORBUFFER_VAL_BAYER_METHOD_MASK

#define CORBUFFER_BAYER_METHOD_ADVANCED      CORBUFFER_VAL_BAYER_METHOD_2

#define CORBUFFER_CONVERT_RANGE_CLIP         CORBUFFER_VAL_CONVERT_RANGE_CLIP
#define CORBUFFER_CONVERT_RANGE_REMAP        CORBUFFER_VAL_CONVERT_RANGE_REMAP
#define CORBUFFER_CONVERT_RANGE_MASK         CORBUFFER_VAL_CONVERT_RANGE_MASK

#define CORBUFFER_CONVERT_COMPLEX_MAG_PHASE  CORBUFFER_VAL_CONVERT_COMPLEX_MAG_PHASE
#define CORBUFFER_CONVERT_COMPLEX_REAL_IMAG  CORBUFFER_VAL_CONVERT_COMPLEX_REAL_IMAG
#define CORBUFFER_CONVERT_COMPLEX_MASK       CORBUFFER_VAL_CONVERT_COMPLEX_MASK

#define CORBUFFER_CONVERT_CHANGE_SIGN        CORBUFFER_VAL_CONVERT_CHANGE_SIGN


#ifdef __cplusplus
extern "C" {
#endif

/* Application programming interface */

/* Buffer creation and destruction */
CORSTATUS CORAPIFUNC CorBufferNew( CORSERVER server, UINT32 memWidth, UINT32 memHeight, UINT32 format, UINT32 type, CORBUFFER *buffer);
CORSTATUS CORAPIFUNC CorBufferNew1D( CORSERVER server, UINT32 memWidth, UINT32 format, UINT32 type, CORBUFFER *buffer);
CORSTATUS CORAPIFUNC CorBufferNew2D( CORSERVER server, UINT32 memWidth, UINT32 memHeight, UINT32 format, UINT32 type, CORBUFFER *buffer);

CORSTATUS CORAPIFUNC CorBufferNewEx( CORSERVER server, UINT32 memWidth, UINT32 memHeight, UINT32 format, UINT32 type, ULONG_PTR physAddress, ULONG_PTR pLinearAddress, CORBUFFER *buffer);
CORSTATUS CORAPIFUNC CorBufferNew1DEx( CORSERVER server, UINT32 memWidth, UINT32 format, UINT32 type, ULONG_PTR physAddress, ULONG_PTR pLinearAddress, CORBUFFER *buffer);
CORSTATUS CORAPIFUNC CorBufferNew2DEx( CORSERVER server, UINT32 memWidth, UINT32 memHeight, UINT32 format, UINT32 type, ULONG_PTR physAddress, ULONG_PTR pLinearAddress, CORBUFFER *buffer);

CORSTATUS CORAPIFUNC CorBufferNewChild( CORBUFFER buffer, UINT32 x, UINT32 y, UINT32 width, UINT32 height, CORBUFFER *child);
CORSTATUS CORAPIFUNC CorBufferNew1DChild( CORBUFFER buffer, UINT32 x, UINT32 width, CORBUFFER *child);
CORSTATUS CORAPIFUNC CorBufferNew2DChild( CORBUFFER buffer, UINT32 x, UINT32 y, UINT32 width, UINT32 height, CORBUFFER *child);

CORSTATUS CORAPIFUNC CorBufferNewFromFile( CORSERVER server, PCSTR filename, UINT32 type, CORBUFFER *buffer);

CORSTATUS CORAPIFUNC CorBufferFree( CORBUFFER buffer);

CORSTATUS CORAPIFUNC CorBufferNewShared( CORSERVER server, UINT32 memWidth, UINT32 memHeight, UINT32 format, UINT32 type, PCSTR szName, CORBUFFER *buffer);
CORSTATUS CORAPIFUNC CorBufferNewSharedEx( CORSERVER server, UINT32 type, PCSTR szName, CORBUFFER *buffer);

/* Buffer attributes */
CORSTATUS CORAPIFUNC CorBufferGetPrm  ( CORBUFFER buffer, UINT32 prm, void *value);
CORSTATUS CORAPIFUNC CorBufferSetPrm  ( CORBUFFER buffer, UINT32 prm, UINT32 value);
CORSTATUS CORAPIFUNC CorBufferSetPrmEx( CORBUFFER buffer, UINT32 prm, const void *value);
CORSTATUS CORAPIFUNC CorBufferGetCap  ( CORBUFFER buffer, UINT32 cap, void *value);

/* Read/Write buffer */
CORSTATUS CORAPIFUNC CorBufferRead ( CORBUFFER buffer, UINT64 offset, void *array, UINT32 size);
CORSTATUS CORAPIFUNC CorBufferWrite( CORBUFFER buffer, UINT64 offset, const void *array, UINT32 size);

CORSTATUS CORAPIFUNC CorBufferReadDots ( CORBUFFER buffer, UINT32 xStart, UINT32 yStart, const PUINT8 dirs, UINT32 nDirs, void *array, UINT32 size);
CORSTATUS CORAPIFUNC CorBufferWriteDots( CORBUFFER buffer, UINT32 xStart, UINT32 yStart, const PUINT8 dirs, UINT32 nDirs, const void *array, UINT32 size);

CORSTATUS CORAPIFUNC CorBufferReadElement ( CORBUFFER buffer, UINT32 xPos, UINT32 yPos, void *element, UINT32 size);
CORSTATUS CORAPIFUNC CorBufferWriteElement( CORBUFFER buffer, UINT32 xPos, UINT32 yPos, const void *element, UINT32 size);

CORSTATUS CORAPIFUNC CorBufferReadElementEx ( CORBUFFER buffer, UINT32 xPos, UINT32 yPos, PCORDATA pData);
CORSTATUS CORAPIFUNC CorBufferWriteElementEx( CORBUFFER buffer, UINT32 xPos, UINT32 yPos, CORDATA data);

CORSTATUS CORAPIFUNC CorBufferReadLine ( CORBUFFER buffer, UINT32 x1, UINT32 y1, UINT32 x2, UINT32 y2, PUINT32 nDatas, void *array, UINT32 size);
CORSTATUS CORAPIFUNC CorBufferWriteLine( CORBUFFER buffer, UINT32 x1, UINT32 y1, UINT32 x2, UINT32 y2, PUINT32 nDatas, const void *array, UINT32 size);

CORSTATUS CORAPIFUNC CorBufferReadRect ( CORBUFFER buffer, UINT32 x, UINT32 y, UINT32 width, UINT32 height, void *array, UINT32 size);
CORSTATUS CORAPIFUNC CorBufferWriteRect( CORBUFFER buffer, UINT32 x, UINT32 y, UINT32 width, UINT32 height, const void *array, UINT32 size);

/* Basic operations */
CORSTATUS CORAPIFUNC CorBufferClear( CORBUFFER buffer, UINT32 value);
CORSTATUS CORAPIFUNC CorBufferClearEx( CORBUFFER buffer, const void *value, UINT32 size);
CORSTATUS CORAPIFUNC CorBufferClearBlack( CORBUFFER buffer);

CORSTATUS CORAPIFUNC CorBufferCopy( CORBUFFER src, UINT32 x, UINT32 y, CORBUFFER dst);
CORSTATUS CORAPIFUNC CorBufferCopyRect( CORBUFFER src, UINT32 xSrc, UINT32 ySrc, UINT32 width, UINT32 height, CORBUFFER dst, UINT32 xDst, UINT32 yDst);

/* (Obsolete) Load/Save from/to file */
CORSTATUS CORAPIFUNC CorBufferLoad( CORBUFFER buffer, PCSTR filename, UINT32 options);
CORSTATUS CORAPIFUNC CorBufferSave( CORBUFFER buffer, PCSTR filename, UINT32 options);

/* Bayer Filter */
CORSTATUS CORAPIFUNC CorBufferBayerConvert( CORBUFFER src, CORBUFFER dst, UINT32 options, CORDATA wb, CORLUT lut);
CORSTATUS CORAPIFUNC CorBufferBayerWhiteBalance( CORBUFFER src, UINT32 options, PCORDATA pWB);

/* Format Conversion */
CORSTATUS CORAPIFUNC CorBufferConvertFormat( CORBUFFER src, CORBUFFER dst, UINT32 options);
CORSTATUS CORAPIFUNC CorBufferSplitComponents( CORBUFFER src, CORBUFFER compA, CORBUFFER compB, CORBUFFER compC, UINT32 options);
CORSTATUS CORAPIFUNC CorBufferMergeComponents( CORBUFFER compA, CORBUFFER compB, CORBUFFER compC, CORBUFFER dst, UINT32 options);

CORSTATUS CORAPIFUNC CorBufferMap( CORBUFFER buffer);
CORSTATUS CORAPIFUNC CorBufferUnmap( CORBUFFER buffer);
CORSTATUS CORAPIFUNC CorBufferMapEx( CORBUFFER buffer, UINT64 offset, SIZE_T size);
CORSTATUS CORAPIFUNC CorBufferMapExEx( CORBUFFER buffer, UINT64 offset, SIZE_T size, void* virtualAddress);

/* Multi buffer API */
CORSTATUS CORAPIFUNC CorBufferMultiNew( CORSERVER server, UINT32 memWidth, UINT32 memHeight, UINT32 format, UINT32 type, CORBUFFERMULTI *buffer, CORBUFFER handles[], UINT32 count);
CORSTATUS CORAPIFUNC CorBufferMultiFree( CORBUFFERMULTI bufferMulti, CORBUFFER handles[]);
//CORSTATUS CORAPIFUNC CorBufferMultiGetPrm  ( CORBUFFERMULTI buffer, UINT32 prm, void *value);

#ifdef  __cplusplus
}
#endif

#endif   // _CAPIBUF_H_
