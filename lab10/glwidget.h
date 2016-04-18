#ifndef __GLWIDGET__INCLUDE__
#define __GLWIDGET__INCLUDE__

#include <QGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMouseEvent>
#include <QTimer>
#include <glm/glm.hpp>
#include <vector>

#define GLM_FORCE_RADIANS

using glm::mat4;
using glm::vec3;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core { 
    Q_OBJECT

    public:
        GLWidget(QWidget *parent=0);
        ~GLWidget();

        GLuint loadShaders(const char* vertf, const char* fragf);
    protected:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();

        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
    private slots:
        void animate();

    private:
        void initializeCube();
        void renderCube(mat4 transform);

        QTimer *timer;

        float time;

        // animation sequence
        std::vector<vec3> rotations;

        // matrices for storing the end result of our animation using different techniques
        mat4 matrixLerp;
        mat4 eulerLerp;
        mat4 quatSlerp;

        GLuint cubeProg;
        GLuint cubeVao;
        GLint cubeProjMatrixLoc;
        GLint cubeViewMatrixLoc;
        GLint cubeModelMatrixLoc;
        GLint cubeTrackballMatrixLoc;

        void initializeGrid();
        void renderGrid();

        GLuint gridProg;
        GLuint gridVao;
        GLint gridProjMatrixLoc;
        GLint gridViewMatrixLoc;
        GLint gridModelMatrixLoc;
        GLint gridTrackballMatrixLoc;

        mat4 projMatrix;
        mat4 viewMatrix;
        mat4 modelMatrix;
        mat4 trackballMatrix;

        int width;
        int height;

        glm::vec3 lastVPt;
        glm::vec3 pointOnVirtualTrackball(const glm::vec2 &pt);
};

#endif
