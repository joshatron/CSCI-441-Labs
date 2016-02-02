HEADERS += glwidget.h 
SOURCES += glwidget.cpp main.cpp

QT += opengl
CONFIG -= app_bundle
CONFIG += console
INCLUDEPATH += "../include"

RESOURCES += \
    shaders.qrc
