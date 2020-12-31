//
// header-start
//////////////////////////////////////////////////////////////////////////////////
//
// \file      triangleimage.cpp
//
// \brief     This file belongs to the C++ tutorial project
//
// \author    Bernard
//
// \copyright Copyright 2019
//            Distributed under the MIT License
//            See http://opensource.org/licenses/MIT
//
//////////////////////////////////////////////////////////////////////////////////
// header-end
//
#include "triangle.h"
#include "triangleimage.h"
#include <QtWidgets>
#include "gse4_util.h"


enum class triangle_t : int {
  wbr = 1,
  wgb = 2,
  wrg = 3
};


//
// Example of variable based template
// this type of template implies code duplication
// when the template in instanciated
// so you have to weight size vs speed benefit
// In our particular case, we could have simply used an extra parameter
//
// Given 3 points p1, p2 and p2
// check if the current point is inside or outside the triangle
// if outside return a black color
// if inside return a color based on barycentric coordinates
// the associated color of each point is defined with the template
// parameter triangle

template<triangle_t triangle>
QRgb sub_triangle(const Triangle &tri, const QPointF current) {
  static GSE4::Clamp<double> clamp_to_rgb(0., 255.);
  //
  // compute the barycentric coordinates
  // as defined in wikipedia
  if (not tri.in_bbox(current)) {
    return Qt::black;
  }

  auto [lambda_p1, lambda_p2, lambda_p3] =  tri.calc_lamdba123(current);


  if (lambda_p1 > 1.0 || lambda_p1 < 0.0 ||
      lambda_p2 > 1.0 || lambda_p2 < 0.0 ||
      lambda_p3 > 1.0 || lambda_p3 < 0.0) {
    return Qt::black;
  } else {
    double r;
    double g;
    double b;
    if (triangle == triangle_t::wbr) {
      // idx = 1 => white - blue - red
      r = (lambda_p1 * 255.0) + (lambda_p2 *   0.0) + (lambda_p3 * 255.0);
      g = (lambda_p1 * 255.0) + (lambda_p2 *   0.0) + (lambda_p3 *   0.0);
      b = (lambda_p1 * 255.0) + (lambda_p2 * 255.0) + (lambda_p3 *   0.0);
    } else if (triangle == triangle_t::wgb) {
      // idx = 2 => white - green - blue
      r = (lambda_p1 * 255.0) + (lambda_p2 *   0.0) + (lambda_p3 *   0.0);
      g = (lambda_p1 * 255.0) + (lambda_p2 * 255.0) + (lambda_p3 *   0.0);
      b = (lambda_p1 * 255.0) + (lambda_p2 *   0.0) + (lambda_p3 * 255.0);
    } else {
      // idx = 3 => white - red - green
      r = (lambda_p1 * 255.0) + (lambda_p2 * 255.0) + (lambda_p3 *   0.0);
      g = (lambda_p1 * 255.0) + (lambda_p2 *   0.0) + (lambda_p3 * 255.0);
      b = (lambda_p1 * 255.0) + (lambda_p2 *   0.0) + (lambda_p3 *   0.0);
    }
    return qRgb( clamp_to_rgb(r), clamp_to_rgb(g), clamp_to_rgb(b));
  }
}

//
// red, green, blue & white colors from BT 709
//

static const QPointF red{0.64, 0.33};
static const QPointF green{0.3, 0.6};
static const QPointF blue{0.15, 0.06};
static const QPointF white{0.3127, 0.32903};

// see wikipedia Barycentric_coordinate_system
// we'll consider the following 3 triangles
// (points are in counter-clockwise order: red, green, blue)
// 1: white - blue - red
// 2: white - green - blue
// 3: white - red - green

static const Triangle tri1(white, blue, red);
static const Triangle tri2(white, green, blue);
static const Triangle tri3(white, red, green);


static
QRgb xy_to_rgb(const QPointF &current) {
  // if the point returned is black
  // we move to the other triangle

  QRgb rgb_tri1 = sub_triangle<triangle_t::wbr>(tri1, current);
  if (rgb_tri1 != Qt::black) {
    return rgb_tri1;
  }

  QRgb rbg_tri2 = sub_triangle<triangle_t::wgb>(tri2, current);
  if (rbg_tri2 != Qt::black) {
    return rbg_tri2;
  }

  QRgb rbg_tri3 = sub_triangle<triangle_t::wrg>(tri3, current);
  return rbg_tri3;
}


// Constructor for a triangle image

TriangleImage::TriangleImage(int width, int height) :
  QImage(width, height, QImage::Format_RGB32) {
  //
  // initialize image with black background
  QPainter painter(this);
  painter.fillRect(rect(), Qt::black);

  // green-ish color
  const QColor gridColor(51, 204, 102);
  QPen gridPen(gridColor);
  painter.setPen(gridPen);

  // draw border
  painter.drawLine(rect().topLeft(), rect().topRight());
  painter.drawLine(rect().topRight(), rect().bottomRight());
  painter.drawLine(rect().bottomRight(), rect().bottomLeft());
  painter.drawLine(rect().bottomLeft(), rect().topLeft());

  // modify the pen for dashes
  QVector<qreal> dashes;
  dashes << 2 << 2;
  gridPen.setDashPattern(dashes);
  painter.setPen(gridPen);

  // draw horizontal/vertical/ lines between bars
  // assume width is normalized to 1.0
  // we draw lines at 0.1, 0.2, ... 0.9
  // i.e 9 vertical or horizontal lines
  // how many vertical
  {
    const int num_of_lines = 9;
    QLine vline(rect().topLeft(), rect().bottomLeft());
    QLine hline(rect().topLeft(), rect().topRight());
    for (int i=1; i<= num_of_lines; ++i) {
      vline.translate(rect().width()/(num_of_lines + 1), 0);
      painter.drawLine(vline);

      hline.translate(0, rect().height()/(num_of_lines + 1));
      painter.drawLine(hline);
    }
  }


  // draw the figure
  // for each pixel in the image at coord (xp, yp)
  // we calculate corresponding the double value  (x,y)
  // with a simple linear transform
  //
  QRgb value;
  double  x_max = 1.00;
  double  y_max = 1.00;
  double  x_factor = static_cast<double>(width -1);
  double  y_factor = static_cast<double>(height -1);
  double  x;
  double  y;

  for (int yp = 0; yp < height; ++yp) {
    y = y_max * static_cast<double>(height - 1 - yp) / y_factor;
    for (int xp = 0; xp < width; ++xp) {
      x = x_max * static_cast<double>(xp) / x_factor;
      value = xy_to_rgb(QPointF{x, y});
      if (value != Qt::black) {
        setPixel(xp, yp, value);
      }
    }
  }
}
