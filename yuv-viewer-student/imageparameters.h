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
#include <iostream>

//
// Singleton
//

class ImageParameters {
 private:
  size_t nb_threads_;
  bool use_simd_;
  bool use_bt709_;
  bool use_spline_;
  ImageParameters() {
    nb_threads_ = 1;
    use_bt709_ = true;
    use_spline_ = false;
    use_simd_ = false;
  }
 public:
  static ImageParameters &instance() {
    static ImageParameters the_object;

    return the_object;
  }
  size_t get_nb_threads() const {
    return nb_threads_;
  }
  bool get_bt709() const {
    return use_bt709_;
  }
  bool get_spline() const {
    return use_spline_;
  }
  bool get_simd() const {
    return use_simd_;
  }


  ImageParameters &set_nb_threads(size_t nb_threads) {
    nb_threads_ = nb_threads;
    return *this;
  }
  ImageParameters &toggle_btflag() {
    use_bt709_ = not use_bt709_;
    return *this;
  }
  ImageParameters &toggle_spline() {
    use_spline_ = not use_spline_;
    return *this;
  }
  ImageParameters &toggle_simd() {
    use_simd_ = not use_simd_;
    return *this;
  }
};
