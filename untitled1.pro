#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T12:10:59
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = untitled1
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += C:\OpenCV\opencv_bin\install\include
LIBS += -L"C:\OpenCV\opencv_bin\install\x64\mingw\bin"
LIBS += -lopencv_calib3d300 -lopencv_core300 -lopencv_features2d300 -lopencv_flann300 \
-lopencv_highgui300 -lopencv_imgcodecs300 -lopencv_imgproc300 -lopencv_ml300 \
-lopencv_objdetect300 -lopencv_photo300 -lopencv_shape300 -lopencv_stitching300 \
-lopencv_superres300 -lopencv_video300 -lopencv_videoio300 -lopencv_videostab300

SOURCES += main.cpp \
    thresholding.cpp \
    hough.cpp

HEADERS += \
    thresholding.h