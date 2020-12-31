// header-start
////////////////////////////////////////////////////////////////////////////////
// \file      decimation.cpp
//
// \brief     low level function for image decimation
//
// \legal     Copyright (c) 2020
//
// \author    Bernard 
//
////////////////////////////////////////////////////////////////////////////////
// header-end

#include "./util.h"
#include "./decimation.h"


 uint nbThread=1;
//! \brief
//!  This function performs the horizontal decimation by 2 of a
//!  source component (16 bits per pixels) into a destination.
//!
//!       source          destination
//!       2*width           width
//!     +--------------+   +------+
//!     |              |   |      |
//!     |              |   |      |
//!     +--------------+   +------+
//!

 void SimpleDecimation::horizontal_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                           uint dst_width, uint src_height,
                           uint dst_stride, uint src_stride) {
	 						 
  //
  uint16_t *src_col0 = src_ptr;
  uint16_t *dst_col0 = dst_ptr;
  for (uint y = 0; y < src_height; ++y) {
    uint16_t *src_row = src_col0;
    uint16_t *dst_row = dst_col0;

    for (uint x = 0; x < dst_width; ++x) {
      *dst_row = *src_row;
      dst_row++;
      src_row +=2;
    }
    src_col0 += src_stride;
    dst_col0 += dst_stride;
  }
  
}


 void SimpleDecimation::do_horizontal_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                           uint dst_width, uint src_height,
                           uint dst_stride, uint src_stride,uint i_,uint max_thread_) {
	 
    uint16_t *src_col0 = src_ptr;
    uint16_t *dst_col0 = dst_ptr;
  
	uint i_begin,i_end;	
	
    i_begin=i_*src_height/max_thread_;
    i_end=(i_+1) *src_height/max_thread_;
	
	for (uint y =i_begin; y < i_end; ++y) {
    uint16_t *src_row = src_col0;
    uint16_t *dst_row = dst_col0;
	 
	for (uint x = 0; x < dst_width; ++x) {
      *dst_row = *src_row;
      dst_row++;
      src_row +=2;
    }
    src_col0 += src_stride;
    dst_col0 += dst_stride;
   }
	
}



//! \brief
//!  This function performs the vertical decimation by 2 of a source component (16 bits per pixels)
//!  into a destination.
//!
//!       source          destination
//!        width           width
//!       +------+       +------+
//!       |      |       |      |   height
//!       |      |       |      |
//!       |      |       +------+
//!       |      |
//!       |      |  2*height
//!       +------+
//!

 void SimpleDecimation::vertical_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                    uint src_width, uint dst_height,
                    uint dst_stride, uint src_stride) {


  //
  uint16_t *src_row0 = src_ptr;
  uint16_t *dst_row0 = dst_ptr;

  for (uint x = 0; x < src_width; ++x) {
    uint16_t *src_col = src_row0;
    uint16_t *dst_col = dst_row0;
    for (uint y = 0; y < dst_height; ++y) {
      *dst_col = *src_col;
      dst_col += dst_stride;
      src_col += 2 * src_stride;
    }
    src_row0++;
    dst_row0++;
  }
  
}

 void SimpleDecimation::do_vertical_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                    uint src_width, uint dst_height,
                    uint dst_stride, uint src_stride,uint i_,uint max_thread_) {


  //
  uint16_t *src_row0 = src_ptr;
  uint16_t *dst_row0 = dst_ptr;
  
  uint i_begin,i_end;	
	
    i_begin=i_*src_width/max_thread_;
    i_end=(i_+1) *src_width/max_thread_;
	

	for (uint x = i_begin; x < i_end; ++x) {
    uint16_t *src_col = src_row0;
    uint16_t *dst_col = dst_row0;
    for (uint y = 0; y < dst_height; ++y) {
      *dst_col = *src_col;
      dst_col += dst_stride;
      src_col += 2 * src_stride;
    }
    src_row0++;
    dst_row0++;
  }

}



void SimpleDecimation::print_method() {
	std::cout<<"Info:downsized image used method decimation simple"<<std::endl;
}