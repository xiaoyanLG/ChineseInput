#-------------------------------------------------
#
# Project created by QtCreator 2017-07-07T16:10:08
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += QT_KEYPAD_NAVIGATION
SOURCES += \
    xyinput.cpp \
    xybordershadowwidget.cpp \
    xytranslateitem.cpp \
    xytranslateview.cpp \
    xytranslatemodel.cpp \
    xymenu.cpp \
    xymenustyle.cpp \
    xytooltips.cpp \
    xydatabaseoperation.cpp

HEADERS  += \
    xyinput.h \
    xybordershadowwidget.h \
    xytranslateitem.h \
    xytranslateview.h \
    xytranslatemodel.h \
    xymenu.h \
    xymenustyle.h \
    xytooltips.h \
    xydatabaseoperation.h

FORMS    += mainwindow.ui

sylixos {
QT += waylandcompositor-private
DEFINES += XYSYLIXOSINPUTMETHOD_LIBRARY
TEMPLATE = lib

SOURCES += \
    xysylixosxyinputmethod.cpp

HEADERS += \
    xysylixosxyinputmethod.h \
    xysylixosxyinputmethod_global.h

} else {
TARGET = XYInput
TEMPLATE = app

SOURCES +=  main.cpp\
        mainwindow.cpp

HEADERS +=  \
    mainwindow.h
}

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    images/images.qrc
