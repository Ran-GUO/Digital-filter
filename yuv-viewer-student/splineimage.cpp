//
// header-start
//////////////////////////////////////////////////////////////////////////////////
//
// \file      splineimage.cpp
//
// \brief     This file belongs to the C++ tutorial project
//
// \author    Bernard
//
// \copyright 2019
//            Distributed under the MIT License
//            See http://opensource.org/licenses/MIT
//
//////////////////////////////////////////////////////////////////////////////////
// header-end
//
#include "splineimage.h"
#include <iostream>
#include <vector>
#include "gse4_util.h"


//
// Spline image constructor
//

SplineImage::SplineImage(int width, int height)
  : QImage(width, height, QImage::Format_RGB32) {
  //
  // initialize image with black background
  //
  QPainter painter(this);
  painter.fillRect(rect(), Qt::black);

  //
  // prepare the spline data
  //
  std::vector<double> xs{   0.,   0.16,   0.42,   0.6425,  0.8575};
  std::vector<double> yr{   0.,  32.  , 237.  , 215.    ,  1.    };
  std::vector<double> yg{   7., 107.  , 255.  , 170.    , 10.    };
  std::vector<double> yb{ 100., 183.  , 235.  ,  40.    , 15.    };

#ifdef USE_SPLINE
  GSE4::Spline spline_r(xs, yr);
  GSE4::Spline spline_g(xs, yg);
  GSE4::Spline spline_b(xs, yb);
#else
  GSE4::PWLinear spline_r(xs, yr);
  GSE4::PWLinear spline_g(xs, yg);
  GSE4::PWLinear spline_b(xs, yb);
#endif

  
  
  // draw the figure
  // for each pixel in the image at coord (xp, yp)
  // we calculate corresponding the double value  (x,y)
  // with a simple linear transform
  //
  double  x_min =  -0.05;
  double  x_max =   1.05;
  double  y_min = -10.00;
  double  y_max = 265.00;

  GSE4::LinearInterpolate h_interpolator(0.0, static_cast<double>(width - 1), x_min, x_max);
  GSE4::LinearInterpolate v_interpolator(y_min, y_max, static_cast<double>(height - 1), 0.0);
  
  
  
  
  GSE4::Clamp<double> clamp_to_rgb(0, 255);
  GSE4::Clamp<int> clamp_to_height(0, height-1);
  
  
  

  // Step 1
  // Main loop
  // - draw vertical colored line
  // - draw R, G & B curves

  for (int xp = 0; xp < width; ++xp) {
    double  x = h_interpolator(xp);

    double dyr = spline_r.get_value(x);
    double dyg = spline_g.get_value(x);
    double dyb = spline_b.get_value(x);
    
    int iyr = clamp_to_rgb(dyr);
    int iyg = clamp_to_rgb(dyg);
    int iyb = clamp_to_rgb(dyb);
    // create a color as a function of the spline value
    // and draw a vertical line
    QColor vcolor(iyr, iyg, iyb);
    painter.setPen(vcolor);
    painter.drawLine(xp, 0, xp, height -1);

    // draw the R, G, B curve points
    // with simple linear interpolation

    int yp_r = static_cast<int>(v_interpolator(dyr));
    setPixel(xp, clamp_to_height(yp_r), qRgb(255, 0, 0));

    int yp_g = static_cast<int>(v_interpolator(dyg));
    setPixel(xp, clamp_to_height(yp_g), qRgb(0, 255, 0));

    int yp_b = static_cast<int>(v_interpolator(dyb));
    setPixel(xp, clamp_to_height(yp_b), qRgb(0, 0, 255));
  }

  // Step 2
  // Draw the grid elements

  QPen white_dashed_pen(Qt::white);
  QVector<qreal> dashes;
  dashes << 2.0 << 2.0;
  white_dashed_pen.setDashPattern(dashes);


  // draw min/max horizontal line
  // in white color dashed
  int yp_bot = static_cast<int>(v_interpolator(0.0));
  int yp_top = static_cast<int>(v_interpolator(255.0));
  painter.setPen(white_dashed_pen);
  painter.drawLine(0, yp_bot, width -1 , yp_bot);
  painter.drawLine(0, yp_top, width -1 , yp_top);

  //
  // draw xi vertical line
  xs.push_back(1.0);
  painter.setPen(Qt::white);

  GSE4::LinearInterpolate hinv_interpolator(x_min, x_max, 0.0, static_cast<double>(width - 1));
  for (auto &xi : xs) {
    int xp = static_cast<int>(hinv_interpolator(xi));
    painter.drawLine(xp, 0, xp, height -1);
  }
}
