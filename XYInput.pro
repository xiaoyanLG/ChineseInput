#-------------------------------------------------
#
# Project created by QtCreator 2017-07-07T16:10:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XYInput
TEMPLATE = app

DEFINES += QT_KEYPAD_NAVIGATION
SOURCES += main.cpp\
        mainwindow.cpp \
    xyinput.cpp \
    xymovablewidget.cpp \
    xybordershadowwidget.cpp \
    xytranslateitem.cpp \
    xytranslatelistmodel.cpp

HEADERS  += mainwindow.h \
    xyinput.h \
    xymovablewidget.h \
    xybordershadowwidget.h \
    xytranslateitem.h \
    xytranslatelistmodel.h

FORMS    += mainwindow.ui

LIBS += -lws2_32 -ladvapi32 -lkernel32 -luser32
