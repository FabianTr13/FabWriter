#-------------------------------------------------
#
# Project created by QtCreator 2016-03-24T23:37:38
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FabWriter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    arbol.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources/Resources.qrc
