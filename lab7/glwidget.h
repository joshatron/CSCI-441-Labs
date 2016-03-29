#ifndef __GLWIDGET__INCLUDE__
#define __GLWIDGET__INCLUDE__

#include <QGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMouseEvent>
#include <tinyobjloader/tiny_obj_loader.h>
#include <glm/glm.hpp>

#define GLM_FORCE_RADIANS

using glm::vec3;
using glm::mat4;
using namespace std;

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

        void keyPressEvent(QKeyEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent *event);

    private:
        void initializeFlatModel(const tinyobj::shape_t &shape,
                                 vector<vec3> &positions,
                                 vector<vec3> &normals);
        void initializeSmoothModel(const tinyobj::shape_t &shape,
                                   vector<vec3> &positions,
                                   vector<vec3> &normals,
                                   vector<unsigned int> &indices);
        void initializeModel(const char* filename);
        void renderModelFlat();
        void renderModelSmooth();

        int numFlatVertices;
        int numSmoothIndices;

        float viewDist;

        GLuint modelProg;
        GLuint modelFlatVao;
        GLuint modelSmoothVao;
        GLint modelProjMatrixLoc;
        GLint modelViewMatrixLoc;
        GLint modelModelMatrixLoc;

        GLint modelLightPosLoc;
        GLint modelLightColorLoc;
        GLint modelLightIntensityLoc;

        GLint modelDiffuseColorLoc;
        GLint modelAmbientColorLoc;

        void initializeGrid();
        void renderGrid();

        GLuint gridProg;
        GLuint gridVao;
        GLint gridProjMatrixLoc;
        GLint gridViewMatrixLoc;
        GLint gridModelMatrixLoc;

        mat4 projMatrix;
        mat4 viewMatrix;
        mat4 modelMatrix;

        int width;
        int height;

        glm::vec3 lastVPt;
        glm::vec3 pointOnVirtualTrackball(const glm::vec2 &pt);

        bool smooth;
};

#endif
