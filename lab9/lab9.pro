HEADERS += glwidget.h 
SOURCES += glwidget.cpp main.cpp

QT += opengl
CONFIG -= app_bundle
CONFIG += console c++11
INCLUDEPATH += "../include"
INCLUDEPATH += $$PWD

RESOURCES += shaders.qrc
