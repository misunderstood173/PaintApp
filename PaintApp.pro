#-------------------------------------------------
#
# Project created by QtCreator 2016-11-29T18:14:51
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PaintApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    shape.cpp \
    eraser.cpp \
    myrectangle.cpp \
    myellipse.cpp \
    connect.cpp \
    myline.cpp \
    pencil.cpp \
    tool.cpp \
    bucket.cpp \
    size.cpp \
    bezier.cpp

HEADERS  += mainwindow.h \
    tool.h \
    shape.h \
    eraser.h \
    myrectangle.h \
    myellipse.h \
    connect.h \
    myline.h \
    pencil.h \
    bucket.h \
    size.h \
    bezier.h

FORMS    += mainwindow.ui \
    connect.ui \
    size.ui

RESOURCES += \
    toolicons.qrc

RC_ICONS = PaintApp.ico
