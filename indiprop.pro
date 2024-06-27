TARGET = indiprop

TEMPLATE = app

CONFIG += thread console

QT += widgets network xml

DESTDIR = bin

RESOURCES = src/led.qrc

OBJECTS_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp

HEADERS += src/indiclient.h   src/mainwindow.h   src/messagewidget.h   src/dockwidget.h   src/treeitem.h
SOURCES += src/indiclient.cpp src/mainwindow.cpp src/messagewidget.cpp src/dockwidget.cpp src/treeitem.cpp src/main.cpp

target.path = $$(OBSHOME)/bin
INSTALLS += target
