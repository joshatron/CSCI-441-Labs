#include "glwidget.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QTextStream>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

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

using namespace std;

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), viewDist(-120) { 
}

GLWidget::~GLWidget() {
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_Space:
            smooth = !smooth;
            if(smooth) {
                cout << "Smooth shaded" << endl;
            } else {
                cout << "Flat shaded" << endl;
            }
            break;
    }
    update();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    makeCurrent();
    viewDist += event->delta();

    if (viewDist > -5.0f) {
        viewDist = -5.0f;
    }

    viewMatrix = lookAt(vec3(0,0,viewDist),vec3(0,0,0),vec3(0,1,0));

    glUseProgram(modelProg);
    glUniformMatrix4fv(modelViewMatrixLoc, 1, false, value_ptr(viewMatrix));

    glUseProgram(gridProg);
    glUniformMatrix4fv(gridViewMatrixLoc, 1, false, value_ptr(viewMatrix));

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
}

void GLWidget::initializeFlatModel(const tinyobj::shape_t &shape,
                                 vector<vec3> &positions,
                                 vector<vec3> &normals) {
    positions.clear();
    normals.clear();

    // Part 1 
    // Make sure you understand the following loop.

    // shape.mesh.indices contains 3 indices for every triangle in the mesh
    // we can loop from 0 to number of triangles (num indices / 3)
    // to iterate over each triangle

    for(size_t tri = 0; tri < shape.mesh.indices.size() / 3; tri++) {
        // Here we're getting the indices for the current triangle
        unsigned int ind0 = shape.mesh.indices[3*tri+0];
        unsigned int ind1 = shape.mesh.indices[3*tri+1];
        unsigned int ind2 = shape.mesh.indices[3*tri+2];

        // Using those indices we can get the three points of the triangle
        vec3 p0 = vec3(shape.mesh.positions[3*ind0+0],
                       shape.mesh.positions[3*ind0+1],
                       shape.mesh.positions[3*ind0+2]);

        vec3 p1 = vec3(shape.mesh.positions[3*ind1+0],
                       shape.mesh.positions[3*ind1+1],
                       shape.mesh.positions[3*ind1+2]);

        vec3 p2 = vec3(shape.mesh.positions[3*ind2+0],
                       shape.mesh.positions[3*ind2+1],
                       shape.mesh.positions[3*ind2+2]);

        // Part 1 - TODO Calculate the normal of the triangle
        vec3 n;

        // Push the points onto our position array
        positions.push_back(p0);
        positions.push_back(p1);
        positions.push_back(p2);

        // Push the normal onto our normal array (once for each point)
        normals.push_back(n);
        normals.push_back(n);
        normals.push_back(n);
    }
}

void GLWidget::initializeSmoothModel(const tinyobj::shape_t &shape,
                                     vector<vec3> &positions,
                                     vector<vec3> &normals,
                                     vector<unsigned int> &indices) {
    positions.clear();
    normals.clear();

    // Part 2 - The following code is just a copy of initializeFlatModel
    // with the exception of the indices array. We can save space in our 
    // position buffer by constructing our primitives using unsigned 
    // integers that reference specific indices in our position buffer.
    // The provided code still duplicates the positions of our vertices,
    // but uses an indices array to specify which vertices form triangles.
    // It mimicks the behavior of glDrawArrays (but is using glDrawElements),
    // by creating an indices array that goes from 0 to 3*(number of triangles)-1. 
    // If you copy over your normal calculation from Part 1, you'll see the model 
    // is still flat shaded because we haven't smoothed the normals yet. 
    // You'll do that below.

    // Part 2a - First, optimize the code below by only adding each vertex position a single
    // time, rather than once per triangle it's used in. This is essentially just copying
    // the positions from shape.mesh.positions into positions.
    
    // Part 2b - Initialize normals to the same size as positions and set each value to
    // (0,0,0) before iterating over each triangle.

    // Part 2c - For every triangle in the model, calculate it's normal and add it to the
    // normal that corresponds to each of the triangle's vertex positions. 
    
    // Part 2d - Populate your indices array with the index data of your mesh. This involves
    // copying over the index array from shape.mesh.indices to your own indices array.

    // Part 2e - Loop over every vertex normal and normalize it. Your normals are now
    // smoothed. 

    for(size_t tri = 0; tri < shape.mesh.indices.size() / 3; tri++) {
        // Here we're getting the indices for the current triangle
        unsigned int ind0 = shape.mesh.indices[3*tri+0];
        unsigned int ind1 = shape.mesh.indices[3*tri+1];
        unsigned int ind2 = shape.mesh.indices[3*tri+2];

        // Using those indices we can get the three points of the triangle
        vec3 p0 = vec3(shape.mesh.positions[3*ind0+0],
                       shape.mesh.positions[3*ind0+1],
                       shape.mesh.positions[3*ind0+2]);

        vec3 p1 = vec3(shape.mesh.positions[3*ind1+0],
                       shape.mesh.positions[3*ind1+1],
                       shape.mesh.positions[3*ind1+2]);

        vec3 p2 = vec3(shape.mesh.positions[3*ind2+0],
                       shape.mesh.positions[3*ind2+1],
                       shape.mesh.positions[3*ind2+2]);

        // Part 1 - TODO Calculate the normal of the triangle
        vec3 n;

        // Push the points onto our position array
        positions.push_back(p0);
        positions.push_back(p1);
        positions.push_back(p2);

        // Push the normal onto our normal array (once for each point)
        normals.push_back(n);
        normals.push_back(n);
        normals.push_back(n);

        indices.push_back(3*tri+0);
        indices.push_back(3*tri+1);
        indices.push_back(3*tri+2);
    }
}

// Initialize all the necessary OpenGL state to be able
// to render a model
void GLWidget::initializeModel(const char* filename) {
    // shapes and materials are required std::vectors that will be populated
    // by tinyobj::LoadObj, see ../include/tinyobjloader for the code
    // Look at ../include/tinyobjloader/tiny_obj_loader.h 
    // for the data structures that make up shape_t, mesh_t and material_t
    // Summary:
    //  A shape consists of a mesh and a name
    //  A mesh is what we're mostly concerned with and contains arrays
    //  for positions, normals, texture coordinates, and indices. 
    //  Normals and texture coordinates only exists if they're stored
    //  in the obj file. The models I've provided don't include normals
    //  and texture coordinates. You will be calculating the normals.
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;

    QFile file(filename);
    string err = tinyobj::LoadObj(shapes, materials, file);

    if(!err.empty()) {
        cerr << err << endl;
        exit(1);
    }

    cout << "# of shapes: " << shapes.size() << endl;
    cout << "# of materials: " << materials.size() << endl;

    vector<vec3> flat_positions;
    vector<vec3> flat_normals;

// Use the first mesh in the obj file to initialize positions and normals for flat shading
    initializeFlatModel(shapes[0], flat_positions, flat_normals);

    numFlatVertices = flat_normals.size();

    vector<vec3> smooth_positions;
    vector<vec3> smooth_normals;
    vector<unsigned int> smooth_indices;

// Use the first mesh in the obj file and to initialize positions, normals and indices
// for smooth shading
    initializeSmoothModel(shapes[0], smooth_positions, smooth_normals, smooth_indices);

    numSmoothIndices = smooth_indices.size();

    // Upload all our data to buffers
    GLuint flatPositionBuffer;
    glGenBuffers(1, &flatPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, flatPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, flat_positions.size()*sizeof(vec3), &flat_positions[0], GL_STATIC_DRAW);

    GLuint flatNormalBuffer;
    glGenBuffers(1, &flatNormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, flatNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, flat_normals.size()*sizeof(vec3), &flat_normals[0], GL_STATIC_DRAW);

    GLuint smoothPositionBuffer;
    glGenBuffers(1, &smoothPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, smoothPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, smooth_positions.size()*sizeof(vec3), &smooth_positions[0], GL_STATIC_DRAW);

    GLuint smoothNormalBuffer;
    glGenBuffers(1, &smoothNormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, smoothNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, smooth_normals.size()*sizeof(vec3), &smooth_normals[0], GL_STATIC_DRAW);

    GLuint smoothIndexBuffer;
    glGenBuffers(1, &smoothIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, smoothIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, smooth_indices.size()*sizeof(unsigned int), &smooth_indices[0], GL_STATIC_DRAW);

    // load our shaders
    GLuint program = loadShaders(":/vert.glsl", ":/frag.glsl");
    glUseProgram(program);
    modelProg = program;

    // get some variable positions
    GLint positionIndex = glGetAttribLocation(program, "position");
    GLint normalIndex = glGetAttribLocation(program, "normal");

    // bind our buffers to vertex array objects and shader attributes
    glGenVertexArrays(1, &modelFlatVao);
    glBindVertexArray(modelFlatVao);

    glBindBuffer(GL_ARRAY_BUFFER, flatPositionBuffer);
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, flatNormalBuffer);
    glEnableVertexAttribArray(normalIndex);
    glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenVertexArrays(1, &modelSmoothVao);
    glBindVertexArray(modelSmoothVao);

    glBindBuffer(GL_ARRAY_BUFFER, smoothPositionBuffer);
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, smoothNormalBuffer);
    glEnableVertexAttribArray(normalIndex);
    glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, smoothIndexBuffer);

    modelProjMatrixLoc = glGetUniformLocation(program, "projection");
    modelViewMatrixLoc = glGetUniformLocation(program, "view");
    modelModelMatrixLoc = glGetUniformLocation(program, "model");

    modelDiffuseColorLoc = glGetUniformLocation(program, "diffuseColor");
    modelAmbientColorLoc = glGetUniformLocation(program, "ambientColor");

    modelLightPosLoc = glGetUniformLocation(program, "lightPos");
    modelLightColorLoc = glGetUniformLocation(program, "lightColor");
    modelLightIntensityLoc = glGetUniformLocation(program, "lightIntensity");

    glUniform3f(modelLightPosLoc, 0,100,0);
    glUniform3f(modelLightColorLoc, 1,1,1);
    glUniform1f(modelLightIntensityLoc, 1);

    glUniform3f(modelAmbientColorLoc, 0, 0, .2);
    glUniform3f(modelDiffuseColorLoc, .25, .8, 1);
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glPointSize(4.0f);

    glEnable(GL_DEPTH_TEST);

    initializeModel(":/models/bunny.obj");

    viewDist = -120;
    viewMatrix = lookAt(vec3(0,0,viewDist),vec3(0,0,0),vec3(0,1,0));

    modelMatrix = mat4(1.0f);

    glUseProgram(modelProg);
    glUniformMatrix4fv(modelViewMatrixLoc, 1, false, value_ptr(viewMatrix));
    glUniformMatrix4fv(modelModelMatrixLoc, 1, false, value_ptr(modelMatrix));

    glUseProgram(gridProg);
    glUniformMatrix4fv(gridViewMatrixLoc, 1, false, value_ptr(viewMatrix));
    glUniformMatrix4fv(gridModelMatrixLoc, 1, false, value_ptr(modelMatrix));
}

void GLWidget::resizeGL(int w, int h) {
    width = w;
    height = h;

    float aspect = (float)w/h;

    projMatrix = perspective(45.0f, aspect, 1.0f, 10000.0f);

    glUseProgram(modelProg);
    glUniformMatrix4fv(modelProjMatrixLoc, 1, false, value_ptr(projMatrix));

    glUseProgram(gridProg);
    glUniformMatrix4fv(gridProjMatrixLoc, 1, false, value_ptr(projMatrix));
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(smooth) {
        renderModelSmooth();
    } else {
        renderModelFlat();
    }
}

void GLWidget::renderModelFlat() {
    glUseProgram(modelProg);
    glBindVertexArray(modelFlatVao);
    glDrawArrays(GL_TRIANGLES, 0, numFlatVertices);
}

void GLWidget::renderModelSmooth() {
    glUseProgram(modelProg);
    glBindVertexArray(modelSmoothVao);
    glDrawElements(GL_TRIANGLES, numSmoothIndices, GL_UNSIGNED_INT, 0);
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
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    makeCurrent();
    vec2 pt(event->x(), event->y());
    vec3 vPt = normalize(pointOnVirtualTrackball(pt));

    vec3 axis = cross(lastVPt, vPt);
    if(length(axis) >= .00001) {
        axis = normalize(axis);
        float angle = acos(dot(vPt,lastVPt));
        mat4 r = rotate(mat4(1.0f), angle, axis);

        modelMatrix = r*modelMatrix;

        glUseProgram(modelProg);
        glUniformMatrix4fv(modelModelMatrixLoc, 1, false, value_ptr(modelMatrix));

        glUseProgram(gridProg);
        glUniformMatrix4fv(gridModelMatrixLoc, 1, false, value_ptr(modelMatrix));
    }
    lastVPt = vPt;
    update();
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
