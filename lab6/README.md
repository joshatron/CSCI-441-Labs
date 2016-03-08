# Lab 6 - Materials and Lights

*You may work in pairs on this assignment. To receive credit, demonstrate your completed program during lab or push your code
up to Bitbucket prior to class on the due date. See the [syllabus](https://bitbucket.org/msucsc441spring2016/syllabus) for an
up-to-date schedule of assignments and due dates.*

In this lab, you’ll be implementing the Phong reflection model to shade the cube from previous labs. The Phong reflection model combines 
diffuse reflections which mimic rough surfaces, specular reflections which mimic shiny surfaces and ambient reflections which mimic the 
small amount of light that is uniformly scattered around the scene.

## Part 1 - Normals

To calculate any of the lighting equations, we need to have the normal at the point we’re shading. The normal is part of the underlying 
geometry. We need to specify the normal at each point, just like we specify position. Modify the initializeCube method to create a normal 
buffer, just like we’re doing for position. You’ll need a normal for every vertex, so I recommend copying the position array and modifying 
the vectors to represent the normal at each point. The normal at each point should be a unit vector that is perpendicular to the face that
the point is a part of. For example, the top face of our cube has a normal of (0,1,0), so all points that define the top face of the cube
should have that normal assigned to them.

## Part 2 - Vertex Shader

We’ll be calculating our lighting equations in our fragment shader, but we’ll need access to the cube’s position and normal data first. 
First, add a normal input variable on your vertex shader. Then, create position and normal outputs in the vertex shader and copy over the 
corresponding data. Be sure to transform the data into eye space first. To transform normal vectors you’ll need to multiply by the 
inverse transpose of the transformation matrix and normalize the resulting vector, whereas your position vectors can just be multiplied by the 
transformation matrix. If you just multiplied the normals by the transformation matrix, you’d see shading errors when your model matrix 
includes scaling.

## Part 3 - Fragment Shader

### Light

#### Position

We’ll need a light source if we want to shade our cube. In your fragment shader, create a uniform vec3 variable that will store our light 
position. Make sure to initialize it in initializeCube (a good starting value is 0,10,0). In your fragment shader, transform the light position
into eye space. Any uniform variables you have in your vertex shader can also be used in your fragment shader as long as you declare them as
uniform variables exactly like you did in your vertex shader (no changes are needed in your C++ code).

#### Color and intensity (optional)

The color of your light can be another uniform vec3 variable that represents the color of your light source. It can be multiplied in to your
lighting equations anywhere material color is multiplied in. If you don't specify a light color, the result will be as if the light is fully
white (1,1,1). Another option would be to add a uniform float intensity variable as a multiplier on the brightness of your light. These are
just ways to adjust the lighting in your scene from your program. For this lab, just do these if you're interested in playing around with it.

### Material

#### Diffuse reflections

Implement diffuse reflections using the light position and the position and normals on your cube. You’ll need to calculate the light direction 
(a unit vector that points in the direction of your light source), and then take the dot product of that vector with your normal. Clamp the dot
product between 0 and 1, as we don’t want any negative values. Multiply the resulting value by the color of your cube and assign that color to
the output color.

#### Specular reflections

To calculate specular reflections we’ll need to calculate the view direction (a unit vector that points toward the camera) and a reflection 
direction (a unit vector of our light direction reflected about the normal). We’ll also need a shininess uniform variable, which is a floating point 
or integer value. The higher the shininess value the shinier the surface (0 would be not shiny and all, 128 would be very shiny). Take the 
dot product of the view direction and reflection direction vectors, clamp the value to between 0 and 1 and raise it to the power of your 
shininess variable. Since specular reflections represents shiny reflections, it should mostly reflect the color of your light source. If you added
a light color, multiply the value you just calculated by the light color and add it to the diffuse color before assigning it to the output color. If
you didn't make a light color variable, just multiply by white (1,1,1).

#### Ambient reflections

Ambient reflections can be implemented by adding a small amount of color to your output color. Use a multiplier such as .1, and multiply it by the
cube's color and add it to the specular and diffuse components before assigning it to the output color.

## Things to notice

When the light is directly above a surface is when the diffuse shading will be brightest. Rotate the cube around and see how the cube looks 
from different angles. If you’ve implemented specular reflections, you should be able to see a specular highlight when the camera is in 
line with the light’s reflection. When the material is very shiny, the highlight will be small, whereas when the material isn’t 
very shiny the highlight will be larger. To maintain realism, you'll need to dial back the intensity of the specular highlight for objects that
aren't very shiny.

## Recommended Reading

[Phong reflection model](http://en.wikipedia.org/wiki/Phong_reflection_model)
