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

double w2nd(double value, int length)
{
    return 1 - value * (2 / length);
}

int main(int argc, char** argv) {
    int width = 640;
    int height = 480;

    cout << "Enter 3 points in form x,y:r,g,b:" << endl;
    string inputs[3];
    cin >> inputs[0];
    cin >> inputs[1];
    cin >> inputs[2];

    double values[3][5];

    for(int k = 0; k < 3; k++)
    {
        cout << inputs[k] << endl;
        vector<string> strs, points, colors;
        boost::split(strs, inputs[k], boost::is_any_of(":"));
        boost::split(points, strs[0], boost::is_any_of(","));
        boost::split(colors, strs[1], boost::is_any_of(","));
        values[k][0] = ::atof(points[0].c_str());
        values[k][1] = ::atof(points[1].c_str());
        values[k][2] = ::atof(colors[0].c_str());
        values[k][3] = ::atof(colors[1].c_str());
        values[k][4] = ::atof(colors[2].c_str());

        cout << "(" << values[k][0] << ", " << values[k][1] << "), R:" << values[k][2] << ", G:" << values[k][3] << ", B:" << values[k][4] << endl;
    }

    double x1 = w2nd(values[0][0], width);
    double x2 = w2nd(values[1][0], width);
    double x3 = w2nd(values[2][0], width);
    double y1 = w2nd(values[0][1], height);
    double y2 = w2nd(values[1][1], height);
    double y3 = w2nd(values[2][1], height);

    double r1 = values[0][2];
    double r2 = values[1][2];
    double r3 = values[2][2];
    double g1 = values[0][3];
    double g2 = values[1][3];
    double g3 = values[2][3];
    double b1 = values[0][4];
    double b2 = values[1][4];
    double b3 = values[2][4];

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
