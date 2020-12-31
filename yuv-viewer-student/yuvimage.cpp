//
// header-start
//////////////////////////////////////////////////////////////////////////////////
//
// \file      yuvimage.cpp
//
// \brief     This file belongs to the C++ tutorial project
//
// \author    Bernard
//
// \copyright Copyright 2019
//
//////////////////////////////////////////////////////////////////////////////////
// header-end
//
#include "yuvimage.h"
#include <QString>
#include <QDebug>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include "gse4_util.h"



// Convert a single yuv point to rgb color
// based on BT.709 formula
// Y = 0.2126*R + 0.7152*G + 0.0722*B
//
//static
//QRgb convert_yuv_to_rgb(uint8_t y, uint8_t u, uint8_t v) {

//  return qRgb( 128, 128, 128);
//}

// Convert raw yuv data to rgb image
// process image pixel by pixel
// calling the conversion yuv to rgb each time
// The raw yuv data is in 4:2:0 format
// i.e a single chroma value (u or v) is applicable to 4 luma values.
// We haven't implemented the filtering for upscaling described in
// https://msdn.microsoft.com/en-us/library/windows/desktop/dd206750(v=vs.85).aspx
//

int YuvImage::trans(int i){
    int rang=(i/width_)/2;
    int colone=(i%width_)/2;
    return colone+rang*(width_/2);
}

//void YuvImage::yuv_to_rgb() {
//
//    R_ = new double [y_size];
//    G_ = new double [y_size];
//    B_ = new double [y_size];

//    for(int i=0; i<y_size; i++) {
//        R_[i] =( 1.1643836*y_raw_[i] +         0*u_raw_[trans(i)] + 1.8765140*v_raw_[trans(i)] - 268.2065015 );
//        G_[i] =( 1.1643836*y_raw_[i] - 0.2132486*u_raw_[trans(i)] - 0.5578116*v_raw_[trans(i)] +  82.8546329 );
//        B_[i] =( 1.1643836*y_raw_[i] + 2.1124018*u_raw_[trans(i)] +         0*v_raw_[trans(i)] - 289.0175656 );
//    }
//}

//MatrixXd deltM(4,4) << 1.1643836, 0.0000000, 1.8765140, -268.2065015, 1.1643836, -0.2132486, -0.5578116, 82.8546329, 1.1643836, 2.1124018, 0, -289.0175656, 0, 0, 0,1;
void YuvImage::yuv_to_rgb() {

    int idx;
    int i = 0;

    r_ = new double [width_*height_];
    g_ = new double [width_*height_];
    b_ = new double [width_*height_];

    for(idx=0 ; idx < width_*height_ ; ++idx) {
        i=YuvImage::trans(idx);
        r_[idx] = 1.1643836*y_raw_[idx] + 0*u_raw_[i] + 1.8765140*v_raw_[i] - 268.2065015;
        g_[idx] = 1.1643836*y_raw_[idx] - 0.2132486*u_raw_[i] - 0.5578116*v_raw_[i] + 82.8546329;
        b_[idx] = 1.1643836*y_raw_[idx] + 2.1124018*u_raw_[i] - 0*v_raw_[i] - 289.0175656;
    }
// Assist function for YUV to RGB image creation
// allocate the memory buffers
// and read raw yuv from the ifstream into the buffers
//
}
void YuvImage::load_from_stream(std::ifstream &yuv_strm) {

//    y_size = width_ * height_;
//    uv_size = y_size >> 2;
//    y_raw_ = new uint8_t [y_size];
//    u_raw_ = new uint8_t [uv_size];
//    v_raw_ = new uint8_t [uv_size];

//    yuv_strm.read(reinterpret_cast<char *>(y_raw_),  y_size);
//    yuv_strm.read(reinterpret_cast<char *>(u_raw_), uv_size);
//    yuv_strm.read(reinterpret_cast<char *>(v_raw_), uv_size);
//    for (int i=0;i<20;i++)
//        std::cout<<oct<<*(y_raw_+i)<<std::endl;
//   std::cout<<sizeof(y_raw_)<<std::endl;
    auto ysize = width_*height_;
    auto uv_size = ysize >> 2;

    y_raw_ = new uint8_t [ysize];
    u_raw_ = new uint8_t [uv_size];
    v_raw_ = new uint8_t [uv_size];

    yuv_strm.read(reinterpret_cast<char*>(y_raw_), ysize);
    yuv_strm.read(reinterpret_cast<char*>(u_raw_), uv_size);
    yuv_strm.read(reinterpret_cast<char*>(v_raw_), uv_size);
}

// Mesure du temps
void YuvImage::time() {

auto time_start = std::chrono::high_resolution_clock::now();
YuvImage::yuv_to_rgb();
auto time_end = std::chrono::high_resolution_clock::now();

auto elapsed_time_us = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start);

// Séparer le temps dans les groupes de 3 chiffres
auto chiffre_1 = elapsed_time_us.count()/1000000;
auto chiffre_2 = elapsed_time_us.count()/1000 - chiffre_1*1000;
auto chiffre_3 = elapsed_time_us.count()- (chiffre_1*1000+chiffre_2)*1000;

std::cout << "INFO: image calculed in "
          << chiffre_1 << "," << chiffre_2 << "," << chiffre_3 <<"us"
          << std::endl;
}
//! \brief
//! Constructor for the YuvImage
//! Note that the default QImage
//! will not be used except if a throw.
//! We shall use QImage::swap at the end

static const int default_width = 400;
static const int default_height = 200;

YuvImage::YuvImage(const std::string &file_name) :
  QImage(default_width, default_height, QImage::Format_RGB32),
  width_{default_width},
  height_{default_height} {
  static GSE4::Clamp<double> clamp_to_rgb(0., 255.);
  // we find the width and height of the file
  // based on the size of the file

  // student version
  // all try catch block removed
  // yuv_strm est un objet de type ifstream qui constrcté par le file entrée
  std::ifstream yuv_strm(file_name, std::ios::in | std::ios::binary);

  // position stream pointer at the end to read the file size...
  yuv_strm.seekg(0, std::ios::end);
  auto filesize =  yuv_strm.tellg();

  // .. and don't forget to put it back to the beg so that we can read
  yuv_strm.seekg(0, std::ios::beg);

  auto ysize = (filesize * 2) / 3;
  // 不能整除
  if (((ysize * 3) / 2) != filesize) {
    // more code is needed here
    qDebug("Wrong size");
  }

  // dumb algorithm for width and height
  switch (ysize) {
    case 3840*2160: width_ = 3840; height_ = 2160; break;
    case 1920*1080: width_ = 1920; height_ = 1080; break;
    case  1024*768: width_ = 1024; height_ =  768; break;
    case   832*480: width_ =  832; height_ =  480; break;
    case   352*288: width_ =  352; height_ =  288; break;
    default: throw wrong_size();
  }
  // to compute  width and height from the size of the file

  // create local image of given size
  // and swap with current image of the class
//  QImage main_image(width_, height_, QImage::Format_RGB32);
//  QPainter painter(&main_image);
//  QPen pen(qRgb(51, 204, 102));
//  painter.setPen(pen);
//  QPoint P1(20,0), P2(20, height_);
//  painter.drawLine(P1,P2);
//  swap(main_image); //charger l'image
//  load_from_stream(yuv_strm);
//  std::cout<<y_size<<std::endl;
//  yuv_to_rgb();
//  int x,y=0;
//  std::cout<<R_[1]<<std::endl;
//  for (int i=0; i<y_size; i++) {
//      x=i%width_;
//      y=i/width_;
//      std::cout<<i<<std::endl;
//      std::cout<<x<<std::endl;
//      std::cout<<y<<std::endl;
//      main_image.setPixelColor(x, y, qRgb( clamp_to_rgb(R_[i]), clamp_to_rgb(G_[i]), clamp_to_rgb(B_[i])));
//      main_image.setPixelColor(x, y, qRgb(R_[i],G_[i],B_[i]));
//  }
//  for (int i=0; i<height_;i++){
//      for (int j=0; j<width_; j++){
//          setPixel(i,j,qRgb(R_[i],G_[i],B_[i]));
//      }
//  }
 // QPainter painter(this);

  //painter.fillRect(rect(), Qt::red);
  // create local image of given size
  // and swap with current image of the class
  QImage main_image(width_, height_, QImage::Format_RGB32);
  swap(main_image);

  for (int yp = 0; yp < height_; ++yp) {
      for (int xp = 0; xp < width_; ++xp) {
          setPixel(xp, yp, qRgb(clamp_to_rgb(r_[xp+yp*width_]), clamp_to_rgb(g_[xp+yp*width_]), clamp_to_rgb(b_[xp+yp*width_])));
      }
  }
  //QPainter painter(this);
  //painter.fillRect(rect(), Qt::blue);

  delete [] y_raw_;
  delete [] u_raw_;
  delete [] v_raw_;

  delete [] r_;
  delete [] g_;
  delete [] b_;
}
