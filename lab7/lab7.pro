HEADERS += glwidget.h 
SOURCES += glwidget.cpp main.cpp ../include/tinyobjloader/tiny_obj_loader.cc

QT += opengl designer
CONFIG -= app_bundle
CONFIG += console c++11
INCLUDEPATH += "../include"
INCLUDEPATH += $$PWD

RESOURCES += shaders.qrc
