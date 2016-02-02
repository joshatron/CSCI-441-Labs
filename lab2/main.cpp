#include <QApplication>
#include <QOpenGLFunctions_3_3_Core>

#include "glwidget.h"
#include <iostream>

using namespace std;

struct Info : QOpenGLFunctions_3_3_Core {
    void printOpenGLInfo();
};

void Info::printOpenGLInfo() {
    initializeOpenGLFunctions();

    const GLubyte* version = glGetString(GL_VERSION);
    cout << version << endl;

    const GLubyte* vendor = glGetString(GL_VENDOR);
    cout << vendor << endl;

    const GLubyte* renderer = glGetString(GL_RENDERER);
    cout << renderer << endl;

    const GLubyte* shaderVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    cout << shaderVersion << endl;

    int numExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

    for(int i = 0; i < numExtensions; i++) {
        const GLubyte* extension = glGetStringi(GL_EXTENSIONS,i);
        cout << extension << endl;
    }
}

int main(int argc, char** argv) {
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setVersion(3,3);  // set a modern version of OpenGL
    format.setProfile(QSurfaceFormat::CoreProfile); 
    QSurfaceFormat::setDefaultFormat(format);

    GLWidget glWidget;
    glWidget.resize(width,height);
    glWidget.show();

    Info f;
    f.printOpenGLInfo();

    return a.exec();
}
