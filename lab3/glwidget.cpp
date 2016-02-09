#include "glwidget.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QTextStream>

using namespace std;


GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), outline(false), drawMode(0) {
    num_pts = 0;
    drawMode = GL_POINTS;
}

GLWidget::~GLWidget() {
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_C:
            cout << "Cleared all the points. Make sure to " 
                    "update this once you modify how the points "
                    "are stored." << endl;
            num_pts = 0;
            glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
            glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
            break;
        case Qt::Key_W:
            outline = !outline;
            if(outline) {
                cout << "Displaying outlines." << endl;
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                cout << "Displaying filled polygons." << endl;
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            break;
        case Qt::Key_Space:
            cout << "Change this to cycle through "
                    "different draw modes and print "
                    "out which one we're currently on." << endl;

            switch (drawMode)
            {
                case GL_POINTS:
                    drawMode = GL_LINES;
                    break;
                case GL_LINES:
                    drawMode = GL_LINE_STRIP;
                    break;
                case GL_LINE_LOOP:
                    drawMode = GL_TRIANGLES;
                    break;
                case GL_TRIANGLE_STRIP:
                    drawMode = GL_TRIANGLE_FAN;
                    break;
                case GL_TRIANGLE_FAN:
                    drawMode = GL_POINTS;
                    break;
            }

            /*
              Cycle between the following draw modes:
                GL_POINTS,
                GL_LINES,
                GL_LINE_STRIP,
                GL_LINE_LOOP,
                GL_TRIANGLES,
                GL_TRIANGLE_STRIP,
                GL_TRIANGLE_FAN

                */
            break;
    }
    update();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
//    if(num_pts >= pts2.size()) {
//        cout << "increasing size by 3" << endl;
//        pts2.resize(pts2.size() + 3);
//    }



    vec2 newPoint;
    newPoint.x = event->x();
    newPoint.y = event->y();

    pts2.push_back(newPoint);

//    pts2[num_pts].x = event->x();
//    pts2[num_pts].y = event->y();

//        pts[num_pts].x = event->x();
//        pts[num_pts].y = event->y();

        cout << "Added point (" << pts2[num_pts].x << ", " << pts2[num_pts].y << ") " << endl;

        num_pts++;

        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(pts), &pts[0], GL_DYNAMIC_DRAW);
        update();


//    } else {
//        cout << "Three points is the max. "
//                "You must change this to make "
//                "it so any number of points can be created." << endl;
//    }
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glPointSize(4.0f);

    // Create a new Vertex Array Object on the GPU which
    // saves the attribute layout of our vertices.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a buffer on the GPU for position data
    glGenBuffers(1, &positionBuffer);

    // Upload the position data to the GPU, storing
    // it in the buffer we just allocated.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

    // Load our vertex and fragment shaders into a program object
    // on the GPU
    program = loadShaders(":/vert.glsl", ":/frag.glsl");

    // Bind the attribute "position" (defined in our vertex shader)
    // to the currently bound buffer object, which contains our
    // position data for a single triangle. This information 
    // is stored in our vertex array object.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glUseProgram(program);
    projectionLoc = glGetUniformLocation(program, "projection");
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0,0,w,h);

    // When our OpenGL context is resized, we need to change
    // our projection matrix to match. Create an orthographic 
    // projection matrix that converts window coordinates to normalized 
    // device coordinates.  This is similar to our previous lab, 
    // except the conversion will happen in our vertex shader. 
    // This way we won't need to do any conversions on our mouse 
    // event coordinates and when we resize the window the geometry 
    // won't be scaled unevenly.

    projection = glm::ortho(0.f, (float)w, (float)h, 0.f);

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    // draw primitives based on the current draw mode
    glDrawArrays(drawMode, 0, num_pts);
    
    // draw points so we can always see them
    // glPointSize adjusts the size of point
    // primitives
    glDrawArrays(GL_POINTS, 0, num_pts);
}

// Copied from LoadShaders.cpp in the the oglpg-8th-edition.zip
// file provided by the OpenGL Programming Guide, 8th edition.
const GLchar* GLWidget::readShader(const char* filename) {
#ifdef WIN32
        FILE* infile;
        fopen_s( &infile, filename, "rb" );
#else
    FILE* infile = fopen( filename, "rb" );
#endif // WIN32

    if ( !infile ) {
#ifdef _DEBUG
        std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
        return NULL;
    }

    fseek( infile, 0, SEEK_END );
    int len = ftell( infile );
    fseek( infile, 0, SEEK_SET );

    GLchar* source = new GLchar[len+1];

    fread( source, 1, len, infile );
    fclose( infile );

    source[len] = 0;

    return const_cast<const GLchar*>(source);
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

