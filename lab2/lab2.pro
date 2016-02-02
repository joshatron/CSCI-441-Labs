HEADERS += glwidget.h 
SOURCES += glwidget.cpp main.cpp

QT += opengl
CONFIG -= app_bundle
CONFIG += console

QMAKE_CXXFLAGS += -DGL_GLEXT_PROTOTYPES

RESOURCES += shaders.qrc
