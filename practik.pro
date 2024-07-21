#-------------------------------------------------
#
# Project created by QtCreator 2024-06-29T14:34:55
#
#-------------------------------------------------

QT       += core gui
QT += network
QT   +=   sql
CONFIG += thread


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = practik
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        mydatagramm.cpp \
        sendthread.cpp

HEADERS  += mainwindow.h \
    mydatagramm.h \
    sendthread.h

FORMS    += mainwindow.ui
