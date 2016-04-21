#ifndef __GLWIDGET__INCLUDE__
#define __GLWIDGET__INCLUDE__

#include <QOpenGLTexture>
#include <QGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMouseEvent>
#include <glm/glm.hpp>
#include <QTimer>

#define GLM_FORCE_RADIANS

using glm::mat4;
using glm::vec3;

struct Particle {
    vec3 position;
    vec3 velocity;
    vec3 acceleration;
    float lifespan;
};

class ParticleSystem {
    public:
        ParticleSystem();
        ~ParticleSystem();

        void emitParticle(Particle p);
        void step(float dt);
        Particle* data();
        unsigned int numParticles();

    private:
        std::vector<Particle> particles;
};

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core { 
    Q_OBJECT

    public:
        GLWidget(QWidget *parent=0);
        ~GLWidget();

        GLuint loadShaders(const char* vertf, const char* fragf);
    private slots:
        void animate();

    protected:
        QTimer *timer;

        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();

        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent *event);

    private:
        float viewDist;

        void initializeCube();
        void renderCube(mat4 transform);
        void renderCubeMipmap(mat4 transform);
        void renderCubeNearestMipmap(mat4 transform);

        ParticleSystem ps;

        void renderParticleSystem(ParticleSystem *ps);

        GLuint textureObject;
        QOpenGLTexture *texMipmap;
        QOpenGLTexture *texNoMipmap;
        QOpenGLTexture *texNearestMipmap;

        GLuint cubeProg;
        GLuint cubeVao;
        GLint cubeProjMatrixLoc;
        GLint cubeViewMatrixLoc;
        GLint cubeModelMatrixLoc;
        GLint cubeTrackballMatrixLoc;
        
        GLuint psProg;
        GLuint psVao;
        GLint psProjMatrixLoc;
        GLint psViewMatrixLoc;
        GLint psModelMatrixLoc;
        GLint psTrackballMatrixLoc;
        GLuint particleBuffer;

        void initializeParticleSystem();

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
        mat4 trackballMatrix;

        int width;
        int height;

        glm::vec3 lastVPt;
        glm::vec3 pointOnVirtualTrackball(const glm::vec2 &pt);
};

#endif
