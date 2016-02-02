#ifndef __GLWIDGET__INCLUDE__
#define __GLWIDGET__INCLUDE__

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class Point {
    public: 
        float x;
        float y;
        Point() : x(0), y(0) {}
        Point(float xx, float yy) : x(xx), y(yy) {}
};
class Color {
    public: 
        float r;
        float g;
        float b;
        Color() : r(0), g(0), b(0) {}
        Color(float rr, float gg, float bb) : r(rr), g(gg), b(bb) {}
};

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

    public:
        GLWidget(QWidget *parent=0);
        ~GLWidget();

        GLuint vao;
        GLuint program;

    protected:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
    private:
        GLuint loadShaders(const char* vertf, const char* fragf);
        Point w2nd(Point pt_w);
};

#endif
