#include "glwidget.h"
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <boost/algorithm/string.hpp>

using namespace std;

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
}

GLWidget::~GLWidget() {
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    
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

    double x1 = w2nd(values[0][0], width, 1);
    double x2 = w2nd(values[1][0], width, 1);
    double x3 = w2nd(values[2][0], width, 1);
    double y1 = w2nd(values[0][1], height, 0);
    double y2 = w2nd(values[1][1], height, 0);
    double y3 = w2nd(values[2][1], height, 0);
    cout << "(" << x1 << ", " << y1 << ")" << endl;
    cout << "(" << x2 << ", " << y2 << ")" << endl;
    cout << "(" << x3 << ", " << y3 << ")" << endl;

    double r1 = values[0][2];
    double r2 = values[1][2];
    double r3 = values[2][2];
    double g1 = values[0][3];
    double g2 = values[1][3];
    double g3 = values[2][3];
    double b1 = values[0][4];
    double b2 = values[1][4];
    double b3 = values[2][4];

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Create a new Vertex Array Object on the GPU which
    // saves the attribute layout of our vertices.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // position data for a single triangle
    Point pts[3] = {
        Point(0,1),
        Point(1,0),
        Point(0,-1)
    };

    // Create a buffer on the GPU for position data
    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);

    // Upload the position data to the GPU, storing
    // it in the buffer we just created.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);

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
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0,0,w,h);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

Point GLWidget::w2nd(Point pt_w) {
    /* convert pt_w to normalized device coordinates */
    /* use this method to convert your input coordinates to
       normalized device coordinates */
    return pt_w;
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
