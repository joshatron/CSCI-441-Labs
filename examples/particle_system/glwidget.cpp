#include "glwidget.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QTextStream>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

using glm::inverse;
using glm::vec2;
using glm::vec3;
using glm::mat4;
using glm::perspective;
using glm::normalize;
using glm::length;
using glm::cross;
using glm::dot;
using glm::rotate;
using glm::value_ptr;
using glm::lookAt;

ParticleSystem::ParticleSystem() {
}

ParticleSystem::~ParticleSystem() {
}

void ParticleSystem::emitParticle(Particle p) {
    particles.push_back(p);
}

Particle* ParticleSystem::data() {
    return &particles[0];
}

unsigned int ParticleSystem::numParticles() {
    return particles.size();
}

bool isDead(const Particle &p) {
    return p.lifespan <= 0;
}

void ParticleSystem::step(float dt) {
    std::vector<Particle>::iterator itr = particles.begin();
    while(itr != particles.end()) {

        itr->position += itr->velocity*dt;
        itr->velocity += itr->acceleration*dt;
        itr->lifespan -= dt;

        itr++;
    }

    particles.erase(remove_if(particles.begin(), particles.end(), isDead), particles.end());
}

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) { 
    viewDist = -10;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer->start(16);

    Particle p;
    p.position = vec3(0,0,0);
    p.velocity = vec3(0,10,0);
    p.acceleration = vec3(0,-10,0);
    p.lifespan = 1.60;

    ps.emitParticle(p);
}

GLWidget::~GLWidget() {
}

void GLWidget::animate() {
    float dt = .016;

    ps.step(dt);
    update();
}

void GLWidget::initializeParticleSystem() {
    glGenVertexArrays(1, &psVao);
    glBindVertexArray(psVao);

    glGenBuffers(1, &particleBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STREAM_DRAW);

    GLuint program = loadShaders(":/ps_vert.glsl", ":/ps_frag.glsl");
    glUseProgram(program);
    psProg = program;

    glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    std::cout << positionIndex << std::endl;
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);

    psProjMatrixLoc = glGetUniformLocation(program, "projection");
    psViewMatrixLoc = glGetUniformLocation(program, "view");
    psModelMatrixLoc = glGetUniformLocation(program, "model");
    psTrackballMatrixLoc = glGetUniformLocation(program, "trackball");

    mat4 identity = mat4(1.0f);
    glUniformMatrix4fv(psViewMatrixLoc, 1, false, value_ptr(identity));
    glUniformMatrix4fv(psModelMatrixLoc, 1, false, value_ptr(identity));
    glUniformMatrix4fv(psTrackballMatrixLoc, 1, false, value_ptr(identity));
}

void GLWidget::initializeGrid() {
    glGenVertexArrays(1, &gridVao);
    glBindVertexArray(gridVao);

    // Create a buffer on the GPU for position data
    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);

    vec3 pts[84];
    for(int i = -10; i <= 10; i++) {

        pts[2*(i+10)] = vec3(i, -.5f, 10);
        pts[2*(i+10)+1] = vec3(i, -.5f, -10);

        pts[2*(i+10)+42] = vec3(10,-.5f, i);
        pts[2*(i+10)+43] = vec3(-10,-.5f, i);
    }

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);

    // Load our vertex and fragment shaders into a program object
    // on the GPU
    GLuint program = loadShaders(":/grid_vert.glsl", ":/grid_frag.glsl");
    glUseProgram(program);
    gridProg = program;

    // Bind the attribute "position" (defined in our vertex shader)
    // to the currently bound buffer object, which contains our
    // position data for a single triangle. This information 
    // is stored in our vertex array object.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    gridProjMatrixLoc = glGetUniformLocation(program, "projection");
    gridViewMatrixLoc = glGetUniformLocation(program, "view");
    gridModelMatrixLoc = glGetUniformLocation(program, "model");
    gridTrackballMatrixLoc = glGetUniformLocation(program, "trackball");
}

void GLWidget::initializeCube() {
    // Create a new Vertex Array Object on the GPU which
    // saves the attribute layout of our vertices.
    glGenVertexArrays(1, &cubeVao);
    glBindVertexArray(cubeVao);

    // Create a buffer on the GPU for position data
    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);

    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);

    vec3 pts[] = {
        // top
        vec3(1,1,1),    // 0
        vec3(1,1,-1),   // 1
        vec3(-1,1,-1),  // 2
        vec3(-1,1,1),   // 3

        // bottom
        vec3(1,-1,1),   // 4
        vec3(-1,-1,1),  // 5
        vec3(-1,-1,-1), // 6
        vec3(1,-1,-1),  // 7

        // front
        vec3(1,1,1),    // 8
        vec3(-1,1,1),   // 9
        vec3(-1,-1,1),  // 10
        vec3(1,-1,1),   // 11

        // back
        vec3(-1,-1,-1), // 12
        vec3(-1,1,-1),  // 13
        vec3(1,1,-1),   // 14
        vec3(1,-1,-1),  // 15

        // right
        vec3(1,-1,1),   // 16
        vec3(1,-1,-1),  // 17
        vec3(1,1,-1),   // 18
        vec3(1,1,1),     // 19

        // left
        vec3(-1,-1,1),  // 20
        vec3(-1,1,1),   // 21
        vec3(-1,1,-1),  // 22
        vec3(-1,-1,-1) // 23

    };

    for(int i = 0; i < 24; i++) {
        pts[i] *= .5;
    }

    vec3 colors[] = {
        // top
        vec3(0,1,0),    
        vec3(0,1,0),    
        vec3(0,1,0),    
        vec3(0,1,0),    

        // bottom
        vec3(0,.5f,0),  
        vec3(0,.5f,0),  
        vec3(0,.5f,0),  
        vec3(0,.5f,0),  

        // front
        vec3(0,0,1),    
        vec3(0,0,1),    
        vec3(0,0,1),    
        vec3(0,0,1),    

        // back
        vec3(0,0,.5f),  
        vec3(0,0,.5f),  
        vec3(0,0,.5f),  
        vec3(0,0,.5f),

        // right
        vec3(1,0,0),    
        vec3(1,0,0),    
        vec3(1,0,0),    
        vec3(1,0,0),    


        // left
        vec3(.5f,0,0),  
        vec3(.5f,0,0),  
        vec3(.5f,0,0),  
        vec3(.5f,0,0)  
    };

    vec2 uvs[] = {
        // top
        vec2(0,0),
        vec2(0,1),
        vec2(1,1),
        vec2(1,0),

        // bottom
        vec2(0,0),
        vec2(0,1),
        vec2(1,1),
        vec2(1,0),

        // front
        vec2(0,0),
        vec2(0,1),
        vec2(1,1),
        vec2(1,0),

        // back
        vec2(0,0),
        vec2(0,1),
        vec2(1,1),
        vec2(1,0),

        // right
        vec2(0,0),
        vec2(0,1),
        vec2(1,1),
        vec2(1,0),

        // left
        vec2(0,0),
        vec2(0,1),
        vec2(1,1),
        vec2(1,0)

    };

    GLuint restart = 0xFFFFFFFF;
    GLuint indices[] = {
        0,1,2,3, restart,
        4,5,6,7, restart,
        8,9,10,11, restart,
        12,13,14,15, restart,
        16,17,18,19, restart,
        20,21,22,23
    };

    // Upload the position data to the GPU, storing
    // it in the buffer we just allocated.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Load our vertex and fragment shaders into a program object
    // on the GPU
    GLuint program = loadShaders(":/vert.glsl", ":/frag.glsl");
    glUseProgram(program);
    cubeProg = program;

    // Bind the attribute "position" (defined in our vertex shader)
    // to the currently bound buffer object, which contains our
    // position data for a single triangle. This information 
    // is stored in our vertex array object.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    GLint colorIndex = glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(colorIndex);
    glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    GLint uvIndex = glGetAttribLocation(program, "uv");
    glEnableVertexAttribArray(uvIndex);
    glVertexAttribPointer(uvIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);

    cubeProjMatrixLoc = glGetUniformLocation(program, "projection");
    cubeViewMatrixLoc = glGetUniformLocation(program, "view");
    cubeModelMatrixLoc = glGetUniformLocation(program, "model");
    cubeTrackballMatrixLoc = glGetUniformLocation(program, "trackball");

    static const GLubyte tex_checkerboard_data[] =
    {
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF
    };

//    QImage img = QImage("cube128.jpg").mirrored(true,true);
    QImage img = QImage("cube128_alpha.png").mirrored(true,true);
//    QImage img = QImage("checkered2.jpg").mirrored(true,true);

    texNoMipmap = new QOpenGLTexture(img, QOpenGLTexture::DontGenerateMipMaps);
    texNearestMipmap = new QOpenGLTexture(img, QOpenGLTexture::GenerateMipMaps);
    texMipmap = new QOpenGLTexture(img, QOpenGLTexture::GenerateMipMaps);

    texNoMipmap->setMinificationFilter(QOpenGLTexture::Nearest);
    texNoMipmap->setMagnificationFilter(QOpenGLTexture::Nearest);

    texNearestMipmap->setMagnificationFilter(QOpenGLTexture::LinearMipMapNearest);
    texNearestMipmap->setMinificationFilter(QOpenGLTexture::LinearMipMapNearest);

    texMipmap->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texMipmap->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);

}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glPointSize(4.0f);

    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

//    glEnable(GL_DEPTH_TEST);
    GLuint restart = 0xFFFFFFFF;
    glPrimitiveRestartIndex(restart);
    glEnable(GL_PRIMITIVE_RESTART);

    initializeGrid();
    initializeCube();
    initializeParticleSystem();

    viewMatrix = lookAt(vec3(0,0,viewDist),vec3(0,0,0),vec3(0,1,0));
    trackballMatrix = mat4(1.0f);

    glUseProgram(cubeProg);
    glUniformMatrix4fv(cubeViewMatrixLoc, 1, false, value_ptr(viewMatrix));
    glUniformMatrix4fv(cubeModelMatrixLoc, 1, false, value_ptr(trackballMatrix));
    glUniformMatrix4fv(cubeTrackballMatrixLoc, 1, false, value_ptr(trackballMatrix));

    glUseProgram(gridProg);
    glUniformMatrix4fv(gridViewMatrixLoc, 1, false, value_ptr(viewMatrix));
    glUniformMatrix4fv(gridModelMatrixLoc, 1, false, value_ptr(trackballMatrix));
    glUniformMatrix4fv(gridTrackballMatrixLoc, 1, false, value_ptr(trackballMatrix));

    glUseProgram(psProg);
    glUniformMatrix4fv(psViewMatrixLoc, 1, false, value_ptr(viewMatrix));
    glUniformMatrix4fv(psModelMatrixLoc, 1, false, value_ptr(trackballMatrix));
    glUniformMatrix4fv(psTrackballMatrixLoc, 1, false, value_ptr(trackballMatrix));
}

void GLWidget::resizeGL(int w, int h) {
    width = w;
    height = h;

    float aspect = (float)w/h;

    projMatrix = perspective(45.0f, aspect, .1f, 100.0f);

    glUseProgram(cubeProg);
    glUniformMatrix4fv(cubeProjMatrixLoc, 1, false, value_ptr(projMatrix));

    glUseProgram(gridProg);
    glUniformMatrix4fv(gridProjMatrixLoc, 1, false, value_ptr(projMatrix));

    glUseProgram(psProg);
    glUniformMatrix4fv(psProjMatrixLoc, 1, false, value_ptr(projMatrix));
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderGrid();
    renderParticleSystem(&ps);

}

void GLWidget::renderParticleSystem(ParticleSystem *ps) {
    glUseProgram(psProg);
    glBindVertexArray(psVao);

    glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle)*ps->numParticles(), ps->data(), GL_STREAM_DRAW);

    glDrawArrays(GL_POINTS, 0, ps->numParticles());
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    makeCurrent();
    viewDist += .01*event->delta();

    if (viewDist > -1.5f) {
        viewDist = -1.5f;
    }

    viewMatrix = lookAt(vec3(0,0,viewDist),vec3(0,0,0),vec3(0,1,0));

    glUseProgram(cubeProg);
    glUniformMatrix4fv(cubeViewMatrixLoc, 1, false, value_ptr(viewMatrix));

    glUseProgram(gridProg);
    glUniformMatrix4fv(gridViewMatrixLoc, 1, false, value_ptr(viewMatrix));
}

void GLWidget::renderGrid() {
    glUseProgram(gridProg);
    glBindVertexArray(gridVao);
    glDrawArrays(GL_LINES, 0, 84);
}

GLuint GLWidget::loadShaders(const char* vertf, const char* fragf) {
    GLuint program = glCreateProgram();

    // read vertex shader from Qt resource file
    QFile vertFile(vertf);
    vertFile.open(QFile::ReadOnly | QFile::Text);
    QString vertString;
    QTextStream vertStream(&vertFile);
    vertString.append(vertStream.readAll());
    std::string vertSTLString = vertString.toStdString();

    const GLchar* vertSource = vertSTLString.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSource, NULL);
    glCompileShader(vertShader);
    {
        GLint compiled;
        glGetShaderiv( vertShader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
            GLsizei len;
            glGetShaderiv( vertShader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( vertShader, len, &len, log );
            std::cout << "Shader compilation failed: " << log << std::endl;
            delete [] log;
        }
    }
    glAttachShader(program, vertShader);

    // read fragment shader from Qt resource file
    QFile fragFile(fragf);
    fragFile.open(QFile::ReadOnly | QFile::Text);
    QString fragString;
    QTextStream fragStream(&fragFile);
    fragString.append(fragStream.readAll());
    std::string fragSTLString = fragString.toStdString();

    const GLchar* fragSource = fragSTLString.c_str();

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSource, NULL);
    glCompileShader(fragShader);
    {
        GLint compiled;
        glGetShaderiv( fragShader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
            GLsizei len;
            glGetShaderiv( fragShader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( fragShader, len, &len, log );
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete [] log;
        }
    }
    glAttachShader(program, fragShader);

    glLinkProgram(program);

    return program;
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    vec2 pt(event->x(), event->y());
    lastVPt = normalize(pointOnVirtualTrackball(pt));

    Particle p;
    p.position = vec3(0,0,0);
    p.velocity = vec3(0,10,0);
    p.acceleration = vec3(0,-10,0);
    p.lifespan = 1.60;

    ps.emitParticle(p);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    vec2 pt(event->x(), event->y());
    vec3 vPt = normalize(pointOnVirtualTrackball(pt));

    vec3 axis = cross(lastVPt, vPt);
//    vec3 axis = cross(vPt, lastVPt);
    if(length(axis) >= .00001) {
        axis = normalize(axis);
        float angle = acos(dot(vPt,lastVPt));
        mat4 r = rotate(mat4(1.0f), angle, axis);

        trackballMatrix = r*trackballMatrix;

        glUseProgram(cubeProg);
        glUniformMatrix4fv(cubeTrackballMatrixLoc, 1, false, value_ptr(trackballMatrix));

        glUseProgram(gridProg);
        glUniformMatrix4fv(gridTrackballMatrixLoc, 1, false, value_ptr(trackballMatrix));

        glUseProgram(psProg);
        glUniformMatrix4fv(psTrackballMatrixLoc, 1, false, value_ptr(trackballMatrix));
    }
    lastVPt = vPt;
}

vec3 GLWidget::pointOnVirtualTrackball(const vec2 &pt) {
    float r = .5f;
    float rr = r*r;
    vec3 p;
    p.x = -1 + pt.x*(2.0f/width);
    p.y = -(float)height/width*(1-pt.y*(2.0f/height));

    float xx = p.x*p.x;
    float yy = p.y*p.y;

    if(xx+yy <= rr*.5) {
        p.z = sqrt(rr-(xx+yy));
    } else {
        p.z = rr*.5/sqrt(xx+yy);
    }

//    std::cout << p.x << ", " << p.y << ", " << p.z << std::endl;

    return p;
}
