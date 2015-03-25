#-------------------------------------------------
#
# Project created by QtCreator 2015-03-22T16:15:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FindShapePro
TEMPLATE = app

INCLUDEPATH += C:\OpenCV\opencv_bin\install\include
LIBS += -L"C:\OpenCV\opencv_bin\install\x64\mingw\bin"
LIBS += -lopencv_calib3d300 -lopencv_core300 -lopencv_features2d300 -lopencv_flann300 \
-lopencv_highgui300 -lopencv_imgcodecs300 -lopencv_imgproc300 -lopencv_ml300 \
-lopencv_objdetect300 -lopencv_photo300 -lopencv_shape300 -lopencv_stitching300 \
-lopencv_superres300 -lopencv_video300 -lopencv_videoio300 -lopencv_videostab300

SOURCES += main.cpp\
        mainwindow.cpp \
    detectorcanny.cpp \
    function.cpp \
    hough.cpp

HEADERS  += mainwindow.h \
    detectorcanny.h \
    function.h \
    hough.h

FORMS    += mainwindow.ui
