// header-start
////////////////////////////////////////////////////////////////////////////////
// \file      decimation_convolution.cpp
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
//! Filter coefficients are calculated by scilab with the following script
//! Fc = 0.249;
//! N = 7;
//! Lobe = 9;
//! Wc = 2*%pi*Fc;
//!
//! function y=my_filter(x)
//!   Cx = (Wc/%pi)*sinc(Wc*x); // Coeff based on sinc(x) = sin(x)/x
//!   Wx = sinc((%pi/Lobe)*x);  // Lanczos Window
//!   y  = Cx .* Wx;
//! endfunction

void ConvolutionDecimation::horizontal_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                           uint dst_width, uint dst_height,
                           uint dst_stride, uint src_stride) {
  //
  int fircoeff[7]={-3026,64,10758,17176,10758,64,-3026}; 
  uint16_t *src_col0 = src_ptr;
  uint16_t *dst_col0 = dst_ptr;
  for (uint y = 0; y < dst_height; ++y) {
	  
    uint16_t *src_row = src_col0;
    uint16_t *dst_row = dst_col0;

    for (uint x = 0; x < dst_width; ++x) {		
		*dst_row = *src_row;
    //filtrage convolution
		int sum=0;
		int i=x;
		for(int k=-3;k<=3;k++){
		int idx=i-k;
		//condition aux limites 
		if (idx<0) idx=0; //gouche
		if (idx>static_cast<int> (dst_width-1)) idx=dst_width-1;//droit
		
		int32_t data=static_cast<int32_t>(src_row[idx]);
		sum+=data*fircoeff[k+3];
		}
		dst_row[x]=(sum+16384)>>15;
	
	
	
    //decimation
     
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

void ConvolutionDecimation::vertical_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                    uint dst_width, uint dst_height,
                    uint dst_stride, uint src_stride) {

  //
   int fircoeff[7]={-3026,64,10758,17176,10758,64,-3026}; 
  uint16_t *src_row0 = src_ptr;
  uint16_t *dst_row0 = dst_ptr;

  for (uint x = 0; x < dst_width; ++x) {

    uint16_t *src_col = src_row0;
    uint16_t *dst_col = dst_row0;
    for (uint y = 0; y < dst_height; ++y) {
		 *dst_col = *src_col;
	    //filtrage convolution
		int sum=0;
		int i=y;
		for(int k=-3;k<=3;k++){
		int idy=i-k;
		//condition aux limites 
		if (idy<0) idy=0; //dessus
		if (idy>static_cast<int> (dst_height-1)) idy=dst_height-1;//dessous
		
		int32_t data=static_cast<int32_t>(src_col[idy]);
		sum+=data*fircoeff[k+3];
		}
		dst_col[y]=(sum+16384)>>15;
		//dst_col[y]=sum/32768;
	
	
	
	//decimation
     
      dst_col += dst_stride;
      src_col += 2 * src_stride;
    }
    src_row0++;
    dst_row0++;
  }
 
}

void ConvolutionDecimation::print_method() {
	std::cout<<"Info:downsized image used method decimation convolution"<<std::endl;
}