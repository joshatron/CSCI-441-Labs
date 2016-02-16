#ifndef __GLWIDGET__INCLUDE__
#define __GLWIDGET__INCLUDE__

#include <QGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMouseEvent>
#include <glm/glm.hpp>

#define GLM_FORCE_RADIANS

using glm::mat4;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core { 
    Q_OBJECT

    public slots:
        void translateX(int x);
        void translateY(int y);
        void translateZ(int z);

        void rotateX(int x);
        void rotateY(int y);
        void rotateZ(int z);

        void scaleX(int x);
        void scaleY(int y);
        void scaleZ(int z);

        void cameraPosition(int z);
        void cameraY(int y);

    public:
        GLWidget(QWidget *parent=0);
        ~GLWidget();

        GLuint loadShaders(const char* vertf, const char* fragf);
    protected:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();

    private:
        float tx;
        float ty;
        float tz;

        float rx;
        float ry;
        float rz;

        float sx;
        float sy;
        float sz;

        float camAngle;
        float camY;

        void updateModelMatrix();
        void updateViewMatrix();

        void initializeCube();
        void renderCube();
        GLuint cubeProg;
        GLuint cubeVao;
        GLint cubeProjMatrixLoc;

        void initializeGrid();
        void renderGrid();
        GLuint gridProg;
        GLuint gridVao;
        GLint gridProjMatrixLoc;

        mat4 projMatrix;
};

#endif
