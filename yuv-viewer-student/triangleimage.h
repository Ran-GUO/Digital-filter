//
// header-start
//////////////////////////////////////////////////////////////////////////////////
//
// \file      triangleimage.h
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

class TriangleImage: public QImage {
public:
  TriangleImage(int width, int height);
  ~TriangleImage() = default;
};
