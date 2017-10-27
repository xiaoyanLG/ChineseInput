#-------------------------------------------------
#
# Project created by QtCreator 2017-07-07T16:10:08
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XYInput
TEMPLATE = app

DEFINES += QT_KEYPAD_NAVIGATION
SOURCES += main.cpp\
        mainwindow.cpp \
    xyinput.cpp \
    xybordershadowwidget.cpp \
    xytranslateitem.cpp \
    xytranslateview.cpp \
    xytranslatemodel.cpp \
    xymenu.cpp \
    xymenustyle.cpp \
    xytooltips.cpp \
    xydatabaseoperation.cpp \
    xyvirtualkeyboard.cpp \
    xypushbutton.cpp

HEADERS  += mainwindow.h \
    xyinput.h \
    xybordershadowwidget.h \
    xytranslateitem.h \
    xytranslateview.h \
    xytranslatemodel.h \
    xymenu.h \
    xymenustyle.h \
    xytooltips.h \
    xydatabaseoperation.h \
    xyvirtualkeyboard.h \
    xypushbutton.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11
