#include "glwidget.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
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
using glm::quat;

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) { 
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer->start(16);

    matrixLerp = mat4(1.0f);
    eulerLerp = mat4(1.0f);
    quatSlerp = mat4(1.0f);

    time = 0;

    // rotations is an array of key frame euler angle orientations
    // feel free to change or add more key frames to the rotations
    // array.
    rotations.push_back(vec3(0, 0, 0));
    rotations.push_back(vec3(M_PI/3, M_PI/2, -4*M_PI));
    rotations.push_back(vec3(M_PI/3, M_PI/2, -4*M_PI));
}

GLWidget::~GLWidget() {
}

void GLWidget::animate() {
    // increment time by our time step
    float dt = .016;
    time += dt;

    // we want 2 seconds of animation per rotation then to start over, so 
    // restart to 0 once we've reached our max time
    if(time > 2*(rotations.size()-1)) {
        time = 0;
    }

    // Convert time to a value between 0 and 1
    // at 0 we're at the beginning of our rotations
    // array, and at 1 we've reach the last one
    float t = fmin(time/(2*(rotations.size()-1)),1);

    // Get two indices into our rotations array
    // that represent our current animation
    unsigned int fromIndex = t*(rotations.size()-1);
    unsigned int toIndex = fromIndex+1;

    // when t = 1 toIndex will be out of range, so
    // just clamp it to the last index in the array
    if(toIndex > rotations.size()-1) {
        toIndex = rotations.size()-1;
    }

    // we want t to be a 0-1 value that represents the
    // percentage between two consecutive indices, so
    // get our current index as a floating point number
    // then subtract off the integer portion
    t = t*(rotations.size()-1)-(int)(t*(rotations.size()-1));

    // Euler angle representations of 
    vec3 from = rotations[fromIndex];
    vec3 to = rotations[toIndex];

    // Part 1 - Variables 'to' and 'from' represent Euler rotations. 
    // Construct two matrices, one using 'from' and one using 'to'.
    // Make sure the order of rotations is rotZ*rotY*rotX to match up
    // with later parts.
    mat4 fromMatrix = rotate(mat4(1.f), from.z, vec3(0,0,1)) * rotate(mat4(1.f), from.y, vec3(0,1,0)) * rotate(mat4(1.f), from.x, vec3(1,0,0));
    mat4 toMatrix = rotate(mat4(1.f), to.z, vec3(0,0,1)) * rotate(mat4(1.f), to.y, vec3(0,1,0)) * rotate(mat4(1.f), to.x, vec3(1,0,0));

    // Then linearly interpolate the two matrices using variable 't',
    // storing the result in matrixLerp. See how matrixLerp is used
    // in paintGL. Compile and run the program to see how 
    matrixLerp = (1 - t) * fromMatrix + t * toMatrix;

    // Part 2 - The next step is to linearly interpolate the two euler
    // angles prior to converting it to a matrix. Create a vec3 variable
    // and use it to store a linearly interpolated combination of from
    // and to.
    //
    // Then construct a matrix using the linearly interpolated Euler angles
    // and store it in eulerLerp. eulerLerp is used in paintGL to render
    // another of the three cubes. Notice how the second cube is just being
    // rotated rather than scaled and skewed.

    // Part 3 - Quaternions are another way to represent orientation. 
    // glm has a quaternion data structure called quat. It's constructor
    // can take a vec3 that represents Euler angles. Construct two quaternions
    // using the from and to euler angles.


    // Interpolate the two quaternions using glm::slerp. slerp stands for
    // spherical linear interpolation and is how quaternions can be animated
    // along the shortest path. glm::slerp takes 3 arguments:
    // glm::slerp(glm::quat q1, glm::quat q2, float t)
    // where t is in the range 0-1 and returns a quaternion t percent
    // between q1 and q2

    // The last step is to convert the resulting quaternion into a matrix
    // for use in our fragment shader. Use glm::toMat4(glm::quat) to do so
    // and store the resulting matrix in quatSlerp. Again, quatSlerp is used
    // in paintGL to render our third cube.

    update();
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

    cubeProjMatrixLoc = glGetUniformLocation(program, "projection");
    cubeViewMatrixLoc = glGetUniformLocation(program, "view");
    cubeModelMatrixLoc = glGetUniformLocation(program, "model");
    cubeTrackballMatrixLoc = glGetUniformLocation(program, "trackball");
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glPointSize(4.0f);

    glEnable(GL_DEPTH_TEST);
    GLuint restart = 0xFFFFFFFF;
    glPrimitiveRestartIndex(restart);
    glEnable(GL_PRIMITIVE_RESTART);

    initializeCube();
    initializeGrid();

    viewMatrix = lookAt(vec3(0,0,-10),vec3(0,0,0),vec3(0,1,0));
    modelMatrix = mat4(1.0f);
    trackballMatrix = mat4(1.0f);

    glUseProgram(cubeProg);
    glUniformMatrix4fv(cubeProjMatrixLoc, 1, false, value_ptr(projMatrix));
    glUniformMatrix4fv(cubeViewMatrixLoc, 1, false, value_ptr(viewMatrix));
    glUniformMatrix4fv(cubeModelMatrixLoc, 1, false, value_ptr(modelMatrix));
    glUniformMatrix4fv(cubeTrackballMatrixLoc, 1, false, value_ptr(trackballMatrix));

    glUseProgram(gridProg);
    glUniformMatrix4fv(gridProjMatrixLoc, 1, false, value_ptr(projMatrix));
    glUniformMatrix4fv(gridViewMatrixLoc, 1, false, value_ptr(viewMatrix));
    glUniformMatrix4fv(gridModelMatrixLoc, 1, false, value_ptr(modelMatrix));
    glUniformMatrix4fv(gridTrackballMatrixLoc, 1, false, value_ptr(trackballMatrix));
}

void GLWidget::resizeGL(int w, int h) {
    width = w;
    height = h;

    float aspect = (float)w/h;

    projMatrix = perspective(45.0f, aspect, 1.0f, 100.0f);

    glUseProgram(cubeProg);
    glUniformMatrix4fv(cubeProjMatrixLoc, 1, false, value_ptr(projMatrix));

    glUseProgram(gridProg);
    glUniformMatrix4fv(gridProjMatrixLoc, 1, false, value_ptr(projMatrix));
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderGrid();

    renderCube(glm::translate(mat4(1.0f), vec3(-2,0,0))*matrixLerp);
    renderCube(glm::translate(mat4(1.0f), vec3(0,0,0))*eulerLerp);
    renderCube(glm::translate(mat4(1.0f), vec3(2,0,0))*quatSlerp);
}

void GLWidget::renderCube(mat4 transform) {
    glUseProgram(cubeProg);
    glBindVertexArray(cubeVao);
    glUniformMatrix4fv(cubeModelMatrixLoc, 1, false, value_ptr(transform));
    glDrawElements(GL_TRIANGLE_FAN, 29, GL_UNSIGNED_INT, 0);
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
    {
        GLint linked;
        glGetProgramiv( program, GL_LINK_STATUS, &linked );
        if ( !linked ) {
            GLsizei len;
            glGetProgramiv( program, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetProgramInfoLog( program, len, &len, log );
            std::cout << "Shader linker failed: " << log << std::endl;
            delete [] log;
        }
    }

    return program;
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    vec2 pt(event->x(), event->y());
    lastVPt = normalize(pointOnVirtualTrackball(pt));
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
