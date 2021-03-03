/*
  Copyright (c) 2018, Detection Technology Inc.
  All right reserved.

  Author: Zhang Xu, 2018-05-07
 */
#ifndef XAREA_SCAN_CORRECT_H
#define XAREA_SCAN_CORRECT_H

#include"ixcorrect.h"

/*
  XAreaScan class is to convert the input line data into are image. The size of single area scan dm is defined as
  _dm_width and _dm_height, and the number of dms in horizontal direction is _tile_width and that of vertical is
  _tile_height, so the physical number of the dms of the area scanning sensor is _tile_height * _tile_width.

  Several tile frames can be combined into a large grid frame, and the horizontal number of tiles is defined as
  _grid_width and that of  vertical is defined as _grid_height.
 */

#define XAREA_PARA_DM_WIDTH  1
#define XAREA_PARA_DM_HEIGHT 2
#define XAREA_PARA_TILE_WIDTH 3
#define XAREA_PARA_TILE_HEIGHT 4
#define XAREA_PARA_GRID_WIDTH 5
#define XAREA_PARA_GRID_HEIGHT 6

class XDLL_EXPORT XAreaScanCorrect : public IXCorrect
{
public:
     XAreaScanCorrect();
     XAreaScanCorrect(uint32_t dm_width, uint32_t dm_height, uint32_t tile_width, uint32_t tile_height,
		      uint32_t grid_width, uint32_t grid_height);
     ~XAreaScanCorrect();

     bool DoCorrect(XImage* image_);
     void SetPara(uint32_t para, uint32_t data);
     void GetPara(uint32_t para, uint32_t& data);
     XImage* GetAreaImage();
private:
     XAreaScanCorrect(const XAreaScanCorrect&);
     XAreaScanCorrect& operator = (const XAreaScanCorrect&);

     uint32_t _dm_width;
     uint32_t _dm_height;
     uint32_t _tile_width;
     uint32_t _tile_height;
     uint32_t _grid_width;
     uint32_t _grid_height;

	uint32_t _pixel_byte;
    uint32_t _raw_line_size;
    uint32_t _dm_line_size;
    uint32_t _dm_size;
    uint32_t _tile_line_size;
    uint32_t _grid_line_size;
    uint32_t _grid_dm_block_size;
    uint32_t _grid_tile_block_size;
    uint32_t _tile_dm_num;
     
     XImage _area_image;

};


#endif //XAREA_SCAN_CORRECT_H
