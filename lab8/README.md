# Lab 8 - Textures

*You may work in pairs on this assignment. To receive credit, demonstrate your completed program during lab or push your code
up to Bitbucket prior to class on the due date. See the [syllabus](https://bitbucket.org/msucsc441spring2016/syllabus) for an
up-to-date schedule of assignments and due dates.*

In this lab, you’ll be exploring the basics of texture mapping. Texture mapping provides added realism when rendering objects. 
Rather than computing all details of a material, they can be looked up in a texture. A texture can simply be an image that 
represents the color of an object, but can also contain other information such as normal, or specular data. 


## Part 1 - Texture coordinates

Texture coordinates are used to look up the information contained within a texture.  Each vertex in your geometry requires a 
texture coordinate. A texture coordinate is a 2 element vector (use glm’s vec2), which has components that range from 0 to 1
Choose coordinates for each vertex so that our texture image is mapped to each face. The x component of your texture coordinate 
maps from the left side (0) of the image to right (1), and the y components maps from the bottom (0) to the top (1). Create an 
array of uv coordinates that correspond to each of the cube’s vertices, upload them to a buffer and bind them to a uv variable 
in your vertex and fragment shaders. This is very similar to what you did for normals in lab 6. Before moving on, make sure your 
cube has uv coordinates by binding the x and y components of your uv coordinate to the red and green components of your output 
color in your fragment shader (i.e something like color_out = vec4(uv.x, uv.y, 0, 1)). Each face of your cube should have a 
black corner (0,0), a red corner (1,0), a green corner (0,1) and a yellow corner (1,1).


## Part 2 - Using textures

First, we need some texture data so let's create a checkerboard texture.

### a. Create texture variables

In initializeGL, define two const variables to the stack, width and height, that will represent the size of our texture.  Then create a one dimensional 
GLuint array, also on the stack, large enough to store all the pixel data for our image (one pixel will be represented with a single GLuint value). 
You'll also need an OpenGL texture object, which you'll want to define as a private member variable on GLWidget (type GLuint). Create the texture object
using glGenTextures, similar to other OpenGL objects (i.e. glGenBuffers, glGenVertexArrays, etc.).

### b. Generate texture data

Fill your texture array with data using two for loops, one for the row and one for the column of each pixel. Each row of pixels is packed end to end in
a one dimensional array, so to access any pixel, given a row and column, you can use the equation, row*width+column. We'll be packing the red, green, blue and alpha channels
of a color into a single 32-bit unsigned integer. OpenGL can accept a few different formats, but we'll stick with this one for the lab, and we'll get to 
where we tell OpenGL this in the next step. For now, you can use 0xFFFFFFFF for white and 0x00000000 for black (every 2 hex values represents one of the 
color channels, so 0xFF000000 would be full red and 0 for every other channel, 0x00FF0000 would be full green and 0 for every other channel, etc., that is 8-bits
for each channel).

### c. Upload texture data to OpenGL

Bind your texture object using glBindTexture(GL_TEXTURE_2D, {texture object variable}). Then upload data to it using glTexImage2D. The parameters in order are:

* target (GL_TEXTURE_2D) - There are different kinds of textures, such as cube maps, but we're just doing a basic 2D texture.
* mip map level (0) - Level 0 is the base image.
* internal format (GL_RGBA) - Our texture data contains red, green, blue and alpha data.
* width (your width variable) - The number of pixels wide your texture is.
* height (your height variable) - The number of pixel tall your texture is.
* border (0) - Must be 0. This is probably left over from an older version OpenGL that is no longer used.
* format (GL_RGBA) - Should match internal format.
* type (GL_UNSIGNED_INT_8_8_8_8) - This is what tells OpenGL that our color data is made up of 8 bits for red, green, blue and alpha all packed into an unsigned integer.
* data (your GLuint array variable) - This is the pointer your texture data.

### d. Set texture sampling behaviors

You must tell OpenGL how it should behave when sampling your texture. The default behavior involves interpolating between neighboring mipmap levels, and since we haven't
uploaded any mipmap levels, we don't want that to happen. Use glTexParameteri to set the behavior to choose the nearest pixel:
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

Other parameters can be set using glTexParameteri, as well, such as how to sample the texture if the uv coordinates are outside the 0-1 range. 
The default is to repeat the texture, but we could instead clamp it to the color at the edge of the texture, for example:

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


### e. Modify fragment shader

Add a uniform sampler2D variable to your fragment shader. By default, the sampler2D variable will be set to 0, which indicates to OpenGL to use GL_TEXTURE0. All our 
texture operations above were performed on GL_TEXTURE0, as it is the default active texture. If you want to use multiple textures you can change the active texture
using glActiveTexture and glUniform1i to change a given sampler2D uniform variable to a different texture. Since we're only using a single texture, you don't need
to worry about any of that. Once you have a uniform sampler2D variable defined in your fragment shader, you can sample it using the texture function (vec4 color = texture({sampler2D variable},
{uv coordinate})). Set the output color of your fragment shader to the sampled color to map the texture image to each face of the cube.

## Part 3 - Loading an image into a texture (optional)

Loading images into a program can be a daunting task given how many different compression techniques and formats there are. Qt simplifies the process by
providing QImage that can load a wide range of different image formats. Qt also provides a QOpenGLTexture object that encapsulates most of the work above.
If you want to load an arbitrary image, create a QOpenGLTexture private member variable on GLWidget (make sure to #include \<QOpenGLTexture\>). In your GLWidget
constructor, initialize the variable using the QOpenGLTexture(Target) constructor (QOpenGLTexture doesn't have a constructor with no arguments, so it must be
initialized this way):

    GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), tex(QOpenGLTexture::Target2D) {
    }

In initializeGL, you can then load a QImage, then pass it to the QOpenGLTexture's setData(QImage) method:
    
    QImage img("image.jpg");
    tex.setData(img.mirrored()); // Qt and OpenGL use opposite directions for the y-axis, so mirrored will flip make the image to the expected orientation

Now all you need to do is use QOpenGLTexture's bind() method or bind(uint) to be able to use it in your shaders:

    tex.bind(); // binds the texture to the currently active texture

    tex.bind(1); // binds the texture to GL_TEXTURE1
    tex.bind(2); // binds the texture to GL_TEXTURE2


## Things to notice

Try scaling multiplying your uv coordinates by 2. Notice how the faces of the cube change. The glTexParameter calls determine how your texture is sampled. 
In this case, we’ve set the wrap behavior of our texture to GL_CLAMP_TO_EDGE. Because we scaled our texture up by 2, a portion of the face is clamped to the 
color at the edge of our texture. Other settings can tell OpenGL to repeat the texture pattern or to mirror it. 

We’ve also set how OpenGL should select a color when the number of pixels in the texture don’t match up exactly with the pixels on the face of the cube. 
We’ve told OpenGL to just pick the nearest color, but you can also tell OpenGL to interpolate the nearest pixels to get a more antialiased look. Experiment 
with the different settings you can pass to glTexParameter. 

## Recommended Reading

* OpenGL Programming Guide, Eightth Edition, Chapter 6
* [Texture Mapping - Wikipedia](http://en.wikipedia.org/wiki/Texture_mapping)

