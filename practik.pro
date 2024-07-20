#-------------------------------------------------
#
# Project created by QtCreator 2024-06-29T14:34:55
#
#-------------------------------------------------

QT       += core gui
QT += network
QT   +=   sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = practik
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        mydatagramm.cpp \
        readthread.cpp

HEADERS  += mainwindow.h \
    mydatagramm.h \
    readthread.h

FORMS    += mainwindow.ui
