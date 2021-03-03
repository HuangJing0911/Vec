/*
  Copyright (c) 2016, Detection Technology Inc.
  All right reserved.

  Author: Zhang Xu, 2016-09-14
*/

#ifndef XTIF_FORMAT_H
#define XTIF_FORMAT_H

#include "iximg_format.h"
#include "ximage.h"
#include "xdevice.h"
/*
  Class XTifFormat provides basic functions to load, save tiff format image.
  It only supports loading the images saved by it.
 */

#define XTIF_BYTE_ORDER_LITTLE    0x4949 //little endian
#define XTIF_BYTE_ORDER_BIG       0x4D4D //big endian
#define XTIF_FLAG          0x2A
#define XTIF_TAG_NUM       21    //Number of tags of IFD
#define XTIF_TAG_ENTRY_MAX 0xFF

//Tags
#define XTIF_TAG_WIDTH     256
#define XTIF_TAG_HEIGHT    257
#define XTIF_TAG_PIXDEPTH  258
#define XTIF_TAG_COMPRESS  259
#define XTIF_TAG_PHOTO_INTERPRE  262
#define XTIF_TAG_STRIP_OFFSET    273
#define XTIF_TAG_ORIENTATION     274
#define XTIF_TAG_SAMPLES_PIXEL   277
#define XTIF_TAG_ROWS_STRIP      278
#define XTIF_TAG_STRIP_COUNT     279
#define XTIF_TAG_DATE_TIME       306
#define XTIF_TAG_GCU_SN          1000
#define XTIF_TAG_GCU_CARD_NUM    1001
#define XTIF_TAG_GCU_CARD_TYPE   1002
#define XTIF_TAG_GCU_INT_TIME    1003
#define XTIF_TAG_GCU_OP_MODE     1004
#define XTIF_TAG_GCU_TEMPERATURE 1005
#define XTIF_TAG_GCU_HUMIDITY    1006
#define XTIF_TAG_GCU_DM_PIXEL    1007
#define XTIF_TAG_GCU_ENERGY_MODE 1008
#define XTIF_TAG_GCU_BIN_MODE	 1009
//Parameter flag for GetPara() and SetPara()
#define XTIF_PARA_DATA     1
#define XTIF_PARA_WIDTH    2
#define XTIF_PARA_HEIGHT   3
#define XTIF_PARA_SN       4
#define XTIF_PARA_PIXEL_DEPTH 5
#define XTIF_PARA_CARD_NUM 6
#define XTIF_PARA_CARD_TYPE 7
#define XTIF_PARA_TEMPER   8
#define XTIF_PARA_HUMIDITY    9
#define XTIF_PARA_DM_PIXEL 10
#define XTIF_PARA_OP_MODE  11
#define XTIF_PARA_INT_TIME 12
#define XTIF_PARA_DATE_TIME 13
#define XTIF_PARA_ENERGY_MODE 14
#define XTIF_PARA_BIN_MODE 15

#define XTIF_SN_LEN        64
#define XTIF_DATE_LEN      32
// TIFF header
typedef struct TifHeader
{
     uint16_t _id;
     uint16_t _version;
     uint32_t _ifd_offset;
}XTIFHeader;

//Tag data offset part could be uint or float
union TIFTagData
{
    uint16_t _short_data; 
	uint32_t _int_data;
     float _float_data;
};

#define XTIF_TAG_TYPE_BYTE 1
#define XTIF_TAG_TYPE_ASC  2
#define XTIF_TAG_TYPE_SHORT 3
#define XTIF_TAG_TYPE_LONG 4
#define XTIF_TAG_TYPE_FLOAT 11

//TIFF tag     
typedef struct TifTag
{
     uint16_t _id;
     uint16_t _data_type;
     uint32_t _data_count;
     TIFTagData _data_offset;
}XTIFTag;

//TIFF IFD
#pragma pack(2)
typedef struct TifIfd
{
     uint16_t _num_entry;
     XTIFTag _tag_list[XTIF_TAG_NUM];
     uint32_t _next_ifd_offset;
}XTIFIfd;

class XDLL_EXPORT XTifFormat : public IXImageFormat
{
public:
     XTifFormat();
     XTifFormat(XImage* image_, XDevice* dev_);
     ~XTifFormat();

     //bool Load1(const char* file_);
     bool Load(const char* file_);
     bool Save(const char* file_);

     bool GetPara(uint32_t para, uint32_t &data);
     bool GetPara(uint32_t para, uint8_t** data_);
     bool GetPara(uint32_t para, float &data);
     
     bool SetPara(uint32_t para, uint32_t data);
     bool SetPara(uint32_t para, uint8_t* data_);
     bool SetPara(uint32_t para, float data);
     
private:
     XTifFormat(const XTifFormat&);
     XTifFormat& operator = (const XTifFormat&);
     void MakeHead();
	 void ConvertByte(uint16_t &data);
     void ConvertByte(uint32_t &data);

     uint8_t* _data_;
     uint32_t _image_width;
     uint32_t _image_height;
     uint32_t _pixel_depth;
     uint32_t _card_num;
     uint32_t _card_type;
     uint32_t _dm_pixel_num;
     uint32_t _op_mode;
     uint32_t _int_time;
	 uint32_t _energy_mode;
	 uint32_t _bin_mode;
     float    _temperature;
     float    _humidity;
     char     _GCU_SN[XTIF_SN_LEN];
     char     _date_time[XTIF_DATE_LEN];
     
     XTIFHeader _header;
     XTIFIfd _ifd;

     bool _is_load;
};
#endif
  
