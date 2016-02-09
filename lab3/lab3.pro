HEADERS += glwidget.h 
SOURCES += glwidget.cpp main.cpp

macx {
	QMAKE_MAC_SDK = macosx10.11
}

QT += opengl
CONFIG -= app_bundle
CONFIG += console
INCLUDEPATH += "../include"

RESOURCES += \
    shaders.qrc
