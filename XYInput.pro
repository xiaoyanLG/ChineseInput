#-------------------------------------------------
#
# Project created by QtCreator 2017-07-07T16:10:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XYInput
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    xyinput.cpp \
    xymovablewidget.cpp \
    xybordershadowwidget.cpp

HEADERS  += mainwindow.h \
    xyinput.h \
    xymovablewidget.h \
    xybordershadowwidget.h

FORMS    += mainwindow.ui
