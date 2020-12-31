//
// header-start
//////////////////////////////////////////////////////////////////////////////////
//
// \file      mainwindow.h
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

#include <QMainWindow>
#include <QBoxLayout>
#include <vector>

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
QT_END_NAMESPACE

enum class ImageKind : int {
  none = 1,
  yuv = 1,
  spline = 2,
  triangle = 0
};

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  static const size_t MAX_RECENT_FILES_;
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override = default;

  QLabel *image_widget_;
  QScrollArea *scroll_area_;

  void resizeEvent(QResizeEvent *event) override;

private slots:
  void slot_exit();
  void slot_about();
  void slot_toggle();
  void slot_use_simd();
  void slot_set_thread();
  void slot_open_file();
  void slot_open_recent_file();
  void slot_load_triangle_image();
  void slot_load_spline_image();
  void slot_load_yuv_image();

private:
  QString base_dir_;
  void update_recent_file_actions();
  void create_actions();
  void create_menus();
  void set_current_file();

  QString stripped_name(const QString &);

  std::vector<QAction *> action_open_recent_files_;
  QAction *action_open_;
  QAction *action_exit_;
  QAction *action_about_;
  QAction *action_toggle_;
  QAction *action_menu_open_recent_title_;
  QAction *action_triangle_image_;
  QAction *action_spline_image_;

  QMenu *menu_open_recent_;

  QString file_name_;
  bool accept_user_event_;
  bool use_default_image_;
  ImageKind image_kind_;

};
