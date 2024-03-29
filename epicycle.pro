######################################################################
# Automatically generated by qmake (3.1) Thu Feb 8 18:42:49 2018
######################################################################

TEMPLATE = app
TARGET = epicycle

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Directory structure
VPATH += src include
INCLUDEPATH += include
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
DESTDIR = bin

# Extra flags
LIBS += -lsequoia -ljack -ljson-c -L/usr/local/lib
QMAKE_CXXFLAGS += -std=gnu++0x

# GDB
QMAKE_CXXFLAGS += -ggdb

# Input
SOURCES += main.cpp
SOURCES += MainWindow.cpp
SOURCES += SequenceManager.cpp
SOURCES += OutportManager.cpp
SOURCES += OutportWidget.cpp
SOURCES += InportManager.cpp
SOURCES += InportWidget.cpp
SOURCES += SequenceEditor.cpp
SOURCES += Button.cpp
SOURCES += NotificationThread.cpp
SOURCES += ClickLabel.cpp
SOURCES += Helper.cpp
SOURCES += Delta.cpp
SOURCES += Dialogs.cpp
SOURCES += TransportWidget.cpp

HEADERS += MainWindow.h
HEADERS += SequenceManager.h
HEADERS += OutportManager.h
HEADERS += OutportWidget.h
HEADERS += InportManager.h
HEADERS += InportWidget.h
HEADERS += SequenceEditor.h
HEADERS += Button.h
HEADERS += NotificationThread.h
HEADERS += ClickLabel.h
HEADERS += Helper.h
HEADERS += Delta.h
HEADERS += Dialogs.h
HEADERS += TransportWidget.h

RESOURCES = epicycle.qrc

target.path = /usr/local/bin
INSTALLS += target

# vim:syntax=sh
