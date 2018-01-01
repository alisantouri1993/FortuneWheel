#-------------------------------------------------
#
# Project created by QtCreator 2017-12-13T13:13:29
#
#-------------------------------------------------
include($$_PRO_FILE_PWD_/deploy.pri)

QT       += core gui

CONFIG+=windeployqt

win32:RC_ICONS += Images/Wheel.png.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Circle
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    luckcircle.cpp

HEADERS  += mainwindow.h \
    luckcircle.h

FORMS    += mainwindow.ui

RESOURCES += \
    circle.qrc
