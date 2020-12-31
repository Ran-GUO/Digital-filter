// header-start
////////////////////////////////////////////////////////////////////////////////
// \file      decimation_vectorized.cpp
//
// \brief     low level function for image decimation using intrinsics
//
// \legal     Copyright (c) 2020
//
// \author    Bernard
//
////////////////////////////////////////////////////////////////////////////////
// header-end


#include <tmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <iostream>
#include <iomanip>

#include "./util.h"
#include "./decimation.h"


#if defined(__GNUC__)
#define ALIGN16(X) X __attribute__((aligned(16)))
#elif defined(_MSC_VER)
#define ALIGN16(X)  __declspec(align(16)) X
#else
#error "Error: not alignment directive"
#endif

#if defined(__GNUC__)
#define ALIGN32(X) X __attribute__((aligned(32)))
#elif defined(_MSC_VER)
#define ALIGN32(X)  __declspec(align(32)) X
#else
#error "Error: not alignment directive"
#endif

static int16_t ALIGN16(mem_fir_coeff[])={0, -3026,64,10758,17176,10758,64,-3026};
static __m128i rconst=_mm_set1_epi32 (16384);
static __m128i r128_coef_fir=_mm_load_si128(reinterpret_cast<__m128i const*>(mem_fir_coeff));
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

void VectorizedDecimation::horizontal_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                           uint dst_width, uint dst_height,
                           uint dst_stride, uint src_stride) {
  
  //int fircoeff[7]={-3026,64,10758,17176,10758,64,-3026}; 
  uint16_t *src_col0 = src_ptr;
  uint16_t *dst_col0 = dst_ptr;
  for (uint y = 0; y < dst_height; ++y) {
	  
    uint16_t *src_row = src_col0;
    uint16_t *dst_row = dst_col0;

    for (uint x = 0; x < dst_width; ++x) {		
		*dst_row = *src_row;
    //filtrage convolution
		//int sum=0;
		int i=x;
		int16_t d[8] ={0};
		for(int k=-3;k<=3;k++){
		int idx=i-k;
		//condition aux limites 
		if (idx<0) idx=0; //gouche
		if (idx>static_cast<int> (dst_width-1)) idx=dst_width-1;//droit
		
		d[k+3]=static_cast<int16_t>(src_row[idx]);
		//sum+=data*fircoeff[k+3];
		}
		int16_t ALIGN16(mem_data[])={d[7],d[6],d[5],d[4],d[3],d[2],d[1],d[0]};
		 __m128i r128_data=_mm_load_si128(reinterpret_cast<__m128i const*>(mem_data));
		 
		 __m128i rdata1=_mm_madd_epi16(r128_data,r128_coef_fir);//* +
		 __m128i rdata2=_mm_hadd_epi32 (rdata1, rconst);//+16384
		 __m128i rdata3=_mm_srai_epi32 (rdata2,15);// >>15
		//float p[4];
		//_mm_storeu_ps(p,rdata3);
		 int sum=0;
		 for(int i=0;i<4;++i)
         sum = sum + _mm_extract_epi32(rdata3, i);		 
		 dst_row[x]=sum;
		//dst_row[x]=(sum+16384)>>15;
	
	
	
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
void VectorizedDecimation::vertical_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                    uint dst_width, uint dst_height,
                    uint dst_stride, uint src_stride) {
  //int fircoeff[7]={-3026,64,10758,17176,10758,64,-3026}; 
 
  uint16_t *src_row0 = src_ptr;
  uint16_t *dst_row0 = dst_ptr;

  for (uint x = 0; x < dst_width; ++x) {

    uint16_t *src_col = src_row0;
    uint16_t *dst_col = dst_row0;
    for (uint y = 0; y < dst_height; ++y) {
		 *dst_col = *src_col;
	    //filtrage convolution
		int i=y;
		int16_t d[8] ={0};
		for(int k=-3;k<=3;k++){
		int idy=i-k;
		//condition aux limites 
		if (idy<0) idy=0; //dessus
		if (idy>static_cast<int> (dst_height-1)) idy=dst_height-1;//dessous
		d[k+3]=static_cast<int16_t>(src_col[idy]);
		
		}
		 int16_t ALIGN16(mem_data[])={d[7],d[6],d[5],d[4],d[3],d[2],d[1],d[0]};
		 __m128i r128_data=_mm_load_si128(reinterpret_cast<__m128i const*>(mem_data));
		 __m128i rdata1=_mm_madd_epi16(r128_data,r128_coef_fir);//* +
		 __m128i rdata2=_mm_add_epi32 (rdata1, rconst);//+16384
		 __m128i rdata3=_mm_srai_epi32 (rdata2, 15);// >>15
		 
		 int sum=0;
		 for(int i=0;i<4;++i)
         sum = sum + _mm_extract_epi32(rdata3, i);		 
		 dst_col[y]=sum;
		
	
	
	//decimation
     
      dst_col += dst_stride;
      src_col += 2 * src_stride;
    }
    src_row0++;
    dst_row0++;
  }
	
	}

					
void VectorizedDecimation::print_method() {
	std::cout<<"Info:downsized image used method decimation vectorized"<<std::endl;
}