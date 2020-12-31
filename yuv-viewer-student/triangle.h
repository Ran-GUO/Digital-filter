//
// header-start
//////////////////////////////////////////////////////////////////////////////////
//
// \file      triangle.h
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
#pragma once
#include <QtWidgets>

class Triangle {
 private:
  QPointF p1_;
  QPointF p2_;
  QPointF p3_;

  double x_min_;
  double x_max_;

  double y_min_;
  double y_max_;

 public:
  Triangle(const QPointF &p1, const QPointF &p2, const QPointF &p3) : p1_{p1}, p2_{p2}, p3_{p3}  {
    x_min_ = std::min({p1_.x(), p2_.x(), p3_.x()});
    x_max_ = std::max({p1_.x(), p2_.x(), p3_.x()});

    y_min_ = std::min({p1_.y(), p2_.y(), p3_.y()});
    y_max_ = std::max({p1_.y(), p2_.y(), p3_.y()});
  }

  bool in_bbox(const QPointF &current) const {
    if (x_min_ <= current.x() && current.x() <= x_max_  &&
        y_min_ <= current.y() && current.y() <= y_max_ ) {
      return true;
    } else {
      return false;
    }
  }

  std::tuple<double, double, double> calc_lamdba123(const QPointF &current) const {

    double det =  (p2_.y() - p3_.y())*(p1_.x() - p3_.x()) + (p3_.x() - p2_.x())*(p1_.y() - p3_.y());
    double lambda1 = (p2_.y() - p3_.y())*(current.x() - p3_.x()) + (p3_.x() - p2_.x())*(current.y() - p3_.y());
    double lambda2 = (p3_.y() - p1_.y())*(current.x() - p3_.x()) + (p1_.x() - p3_.x())*(current.y() - p3_.y());

    double lambda3 = det - lambda1 - lambda2;

    return std::make_tuple(lambda1 / det, lambda2 / det, lambda3 / det );
  }

};


