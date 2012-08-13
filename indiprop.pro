TARGET = cliq

TEMPLATE = app

CONFIG += thread console
CONFIG -= debug

QT += network xml

DESTDIR = bin

RESOURCES = src/led.qrc

OBJECTS_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp

HEADERS += src/indiclient.h   src/mainwindow.h
SOURCES += src/indiclient.cpp src/mainwindow.cpp src/main.cpp

