#-------------------------------------------------
#
# Project created by QtCreator 2015-01-07T21:40:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yuv-viewer-student
TEMPLATE = app

CONFIG += c++1y

SOURCES += main.cpp \
    mainwindow.cpp \
    splineimage.cpp \
    triangleimage.cpp \
    yuvimage.cpp

HEADERS  += mainwindow.h \
    yuvimage.h \
    splineimage.h \
    triangleimage.h \
    gse4_util.h \
    imageparameters.h


QMAKE_CXXFLAGS += -std=c++1y -mavx -Wno-attributes -Wno-sign-compare

QMAKE_CXXFLAGS_DEBUG   = -g
QMAKE_CXXFLAGS_RELEASE = -O3

#QMAKE_CXXFLAGS += -DUSE_SPLINE

INCLUDEPATH += C:\cygwin64\home\usr\local\include\eigen-eigen-323c052e1731
