//
// header-start
//////////////////////////////////////////////////////////////////////////////////
//
// \file      splineimage.h
//
// \brief     This file belongs to the C++ tutorial project
//
// \author    Bernard Plessier
//
// \copyright 2019
//            Distributed under the MIT License
//            See http://opensource.org/licenses/MIT
//
//////////////////////////////////////////////////////////////////////////////////
// header-end
//

#pragma once

#include <QtWidgets>

QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE


class SplineImage : public QImage {
public:
  SplineImage(int width, int height);
  ~SplineImage() = default;
};
