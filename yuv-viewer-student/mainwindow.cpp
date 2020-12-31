//
// header-start
//////////////////////////////////////////////////////////////////////////////////
//
// \file      mainwindow.cpp
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
#include "imageparameters.h"
#include "mainwindow.h"
#include "yuvimage.h"
#include "triangleimage.h"
#include "splineimage.h"
#include <QtWidgets>


//
// Initialization of the static member
// common to all objects of the class
//

const size_t MainWindow::MAX_RECENT_FILES_ = 10;

//
// MainWindow constructor
//

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  image_kind_ = ImageKind::none;
  //
  // construct image_widget and scroll area
  //
  image_widget_ = new QLabel;
  image_widget_->setBackgroundRole(QPalette::Base);
  image_widget_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  image_widget_->setScaledContents(false);
  image_widget_->setContentsMargins(0,0,0,0);
  image_widget_->setMinimumSize(800, 600);
  QPainter painter(image_widget_);
  painter.fillRect(image_widget_->rect(), Qt::gray);


  scroll_area_ = new QScrollArea;
  scroll_area_->setBackgroundRole(QPalette::Dark);
  scroll_area_->setWidget(image_widget_);
  scroll_area_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scroll_area_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scroll_area_->setWidgetResizable(true);

  setCentralWidget(scroll_area_);

  resize(QGuiApplication::primaryScreen()->availableSize() * 2 / 5);

  base_dir_ = QDir::homePath();

  create_actions();
  create_menus();
  update_recent_file_actions();


}


void MainWindow::create_actions() {
  //
  action_open_ = new QAction(tr("&Open Yuv File"), this);
  action_open_->setShortcut(tr("Ctrl+O"));
  connect(action_open_, SIGNAL(triggered()), this, SLOT(slot_open_file()));

  //action_menu_open_recent_title_ = new QAction(tr("Recently Opened Files"), this);
  action_triangle_image_ = new QAction(tr("Open Triangle Image"), this);
  connect(action_triangle_image_, SIGNAL(triggered()), this, SLOT(slot_load_triangle_image()));

  action_spline_image_ = new QAction(tr("Open Spline Image"), this);
  connect(action_spline_image_, SIGNAL(triggered()), this, SLOT(slot_load_spline_image()));
  

  action_exit_ = new QAction(tr("E&xit"), this);
  action_exit_->setShortcut(tr("Ctrl+Q"));
  connect(action_exit_, SIGNAL(triggered()), this, SLOT(slot_exit()));


  action_about_ = new QAction(tr("&About"), this);
  connect(action_about_, SIGNAL(triggered()), this, SLOT(slot_about()));

}

void MainWindow::slot_exit() {
  close();
}

void MainWindow::slot_about() {
    QMessageBox::about(this, tr("About Combo Viewer"),
            tr("<p><b>yuv-viewer</b> display on the screen an YUV 4:2:0 image "
               "read from a given file.</p>"));
}

void MainWindow::slot_toggle() {
  qDebug() << "INFO: slot_toggle";
  if (image_kind_ == ImageKind::yuv) {
    ImageParameters::instance().toggle_btflag();
    slot_load_yuv_image();
  } else if (image_kind_ == ImageKind::spline) {
    ImageParameters::instance().toggle_spline();
    slot_load_spline_image();
  }
}

void MainWindow::slot_use_simd() {
  qDebug() << "INFO: slot_use_simd";
  if (image_kind_ == ImageKind::yuv) {
    ImageParameters::instance().toggle_simd();
    slot_load_yuv_image();
  }
}

void MainWindow::slot_set_thread() {
  QAction *action = qobject_cast<QAction *>(sender());
  if (action) {
    int nb_thread = action->data().toInt();
    qDebug() << "INFO: slot_thread:" << nb_thread;

    if (image_kind_ == ImageKind::yuv) {
      ImageParameters::instance().set_nb_threads(nb_thread);
      slot_load_yuv_image();
    }
  }
}

void MainWindow::create_menus() {
  auto *menu_file = new QMenu(tr("&File"), this);

  auto *menu_open_recent = new QMenu(menu_file);
  menu_open_recent->setTitle(tr("Open Recent"));

  menu_file->addAction(action_open_);
  menu_file->addAction(menu_open_recent->menuAction());
  menu_file->addSeparator();
  menu_file->addAction(action_triangle_image_);
  menu_file->addAction(action_spline_image_);
  menu_file->addAction(action_exit_);


  for (size_t i = 0; i < MAX_RECENT_FILES_; ++i) {
    auto qaction = new QAction(this);
    action_open_recent_files_.push_back(qaction);

    qaction->setVisible(false);
    menu_open_recent->addAction(qaction);

    connect(qaction, SIGNAL(triggered()), this, SLOT(slot_open_recent_file()));
  }

  //
  // Parameters menu
  // 1) thread
  // 2) toggle
  QMenu *menu_parameters = new QMenu(tr("&Parameters"), this);

  QMenu *menu_set_thread = new QMenu(menu_parameters);
  menu_set_thread->setTitle(tr("Set Threads"));

  for (int i = 1; i <= 10; ++i) {
    auto qaction = new QAction(this);
    QString text = tr("%1").arg(i);
    qaction->setText(text);
    qaction->setData(i);
    qaction->setVisible(true);
    menu_set_thread->addAction(qaction);
    connect(qaction, SIGNAL(triggered()), this, SLOT(slot_set_thread()));
  }

  auto action_toggle = new QAction(tr("&Toggle Image"), this);
  connect(action_toggle, SIGNAL(triggered()), this, SLOT(slot_toggle()));
  menu_parameters->addAction(action_toggle);

  auto action_use_simd = new QAction(tr("&Simd on/off"), this);
  connect(action_use_simd, SIGNAL(triggered()), this, SLOT(slot_use_simd()));
  menu_parameters->addAction(action_use_simd);

  menu_parameters->addAction(menu_set_thread->menuAction());


  QMenu *menu_help = new QMenu(tr("&Help"), this);
  menu_help->addAction(action_about_);

  menuBar()->addMenu(menu_file);
  menuBar()->addMenu(menu_parameters);
  menuBar()->addMenu(menu_help);
}

// This is a virtual function override
// base class behavior only
// When the window has change size, print size on the console

void MainWindow::resizeEvent(QResizeEvent * /* event */) {
  qDebug() << "INFO: resize width  is " << width();
  qDebug() << "INFO: resize height is " << height();

}

///////////////////////////////
//
// YUV Image
//
///////////////////////////////


void MainWindow::slot_open_recent_file() {
  QAction *action = qobject_cast<QAction *>(sender());
  if (action) {
    file_name_ = action->data().toString();
    slot_load_yuv_image();
  }
}


void MainWindow::slot_open_file() {
  file_name_ =
      QFileDialog::getOpenFileName(this,
                                   tr("Open Raw File"),
                                   base_dir_,
                                   tr("Images (*.yuv);;Any files (*)"));

  if (file_name_.isEmpty()) {
    // may be the result of a "cancel" action in the fileDialog
    return;
  }
  // update the base_dir so that we open from the last location
  // next time
  base_dir_ = QFileInfo(file_name_).canonicalPath();

  set_current_file();
  slot_load_yuv_image();
}

// Entry point to process a yuv file
// call the YuvImage constructor
// update the window title with the file name
// copy the image to the image_widget for display

void MainWindow::slot_load_yuv_image() {
  image_kind_ = ImageKind::yuv;

  YuvImage image(file_name_.toStdString());
  QString window_title = QString("%1 %2x%3").arg(file_name_).arg(image.width()).arg(image.height());
  setWindowFilePath(window_title);

  image_widget_->setPixmap(QPixmap::fromImage(image));
  image_widget_->setFixedSize(image.width(), image.height());
  image_widget_->adjustSize();

  adjustSize();
}


//
// The methods below manage the recent file list
// history. We use persistent storage settings.value(key)....

void MainWindow::set_current_file() {
  QString key("recentFileList");

  // query file list from key "recentFileList"
  // move fileName to top of the list
  // and remove other instance of fileName

  QSettings settings;
  QStringList files = settings.value(key).toStringList();
  files.removeAll(file_name_);
  files.prepend(file_name_);
  while (files.size() > MAX_RECENT_FILES_) {
    files.removeLast();
  }
  settings.setValue(key, files);

  foreach (QWidget *widget, QApplication::topLevelWidgets()) {
    MainWindow *main_window = qobject_cast<MainWindow *>(widget);
    if (main_window) {
      main_window->update_recent_file_actions();
    }
  }
}

void MainWindow::update_recent_file_actions() {
  QSettings settings;
  QString key("recentFileList");

  QStringList files = settings.value(key).toStringList();

  size_t numRecentFiles = std::min<size_t>(files.size(), MAX_RECENT_FILES_);

  for (size_t i = 0; i < numRecentFiles; ++i) {
    QString text = tr("%1").arg(stripped_name(files[i]));
    action_open_recent_files_[i]->setText(text);
    action_open_recent_files_[i]->setData(files[i]);
    action_open_recent_files_[i]->setVisible(true);
  }
  for (size_t i = numRecentFiles; i < MAX_RECENT_FILES_; ++i) {
    action_open_recent_files_[i]->setVisible(false);
  }
}

QString MainWindow::stripped_name(const QString &full_file_name) {
  // ensure that we keep at most 50 characters
  //
  QString file_name("/" + QFileInfo(full_file_name).fileName());
  QString path_name(QFileInfo(full_file_name).canonicalPath());

  if (path_name.length() > 6) {
    int  l_path_name = path_name.length();
    int  l_file_name = file_name.length();

    if (l_path_name + l_file_name > 50) {
      int l_left = 50 - (l_file_name + 3);
      l_left = (l_left < 3)  ? 3 : l_left;
      // remove from l_path_name
      path_name = path_name.left(l_left).append("...");
    }
  }
  return path_name + file_name;
}

///////////////////////////////
//
// Colored Triangle
//
///////////////////////////////

const int triangle_width = 600;
const int triangle_height = 600;

void MainWindow::slot_load_triangle_image() {
  image_kind_ = ImageKind::triangle;

  TriangleImage triangle_image(triangle_width, triangle_height);

  image_widget_->setPixmap(QPixmap::fromImage(triangle_image));
  image_widget_->setFixedSize(triangle_width, triangle_height);
  // hack to prevent scroll bar
  scroll_area_->setFixedSize(triangle_width + 2, triangle_height + 2);
  adjustSize();
}


///////////////////////////////
//
// Spline Colored Rectangle
//
///////////////////////////////
const int spline_width = 1024;
const int spline_height = 512;

void MainWindow::slot_load_spline_image() {
  image_kind_ = ImageKind::spline;

  SplineImage spline_image(spline_width, spline_height);

  // the Qimage is now drawn
  // we set the image_widget_ to hold the image
  // and adjust the widgets to fit the image

  image_widget_->setPixmap(QPixmap::fromImage(spline_image));
  image_widget_->setFixedSize(spline_width, spline_height);
  // hack to prevent scroll bar
  scroll_area_->setFixedSize(spline_width + 2, spline_height + 2);
  adjustSize();
}


