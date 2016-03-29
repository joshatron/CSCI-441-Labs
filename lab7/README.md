# Lab 7 - Smooth vs Flat Shading

*You may work in pairs on this assignment. To receive credit, demonstrate your completed program during lab or push your code
up to Bitbucket prior to class on the due date. See the [syllabus](https://bitbucket.org/msucsc441spring2016/syllabus) for an
up-to-date schedule of assignments and due dates.*

Flat and smooth shading are used to describe how the normals of a mesh are used to shade a model. When flat shading, each 
face of a mesh uses its true normal in lighting calculations which results in a very faceted look. When smooth shading, the 
normal at each vertex is an average normal of all the faces that are constructed using that vertex. When using the averaged 
normal in lighting calculations, the mesh appears curved, or smoothed. 

In this lab, you’ll be loading mesh data from an obj file and shading it using both flat and smooth shading. An obj file 
represents an indexed face set, which is a list of vertices followed by a list of faces. Each face is defined by three 
indices that reference the previously defined vertices to define a triangle. You will need to iterate over this data, 
compute normals and organize the data into OpenGL buffers so you can switch between a smooth shaded render and a flat 
shaded render by pressing the spacebar.

## Part 1 - Normals for Flat Shading

Just like last lab, we’ll need normals in order to calculate our shading equations. This time we can’t just hard-code a 
list of normals like we did for the cube. Instead, we need to calculate the normals based on the three points that make 
up each triangle. In the provided code, we use an obj loader library to read in the mesh data, positions and indices, that 
represent a list of triangles. The initializeFlatModel method initializes your model for flat shading, but leaves out the 
normal calculation. It’s your job to calculate the normal of each triangle. To do so, pick a single point of any given 
triangle and calculate the two vectors from that point to the other two points of the triangle. Take the cross product of 
those two points and normalize the result to get the normal. Once your positions and normals array are populated, the mesh 
is rendered using glDrawArrays with a draw mode of GL_TRIANGLES (see renderModelFlat).

## Part 2 - Normals For Smooth Shading

To implement smooth shading on your model, you’ll need to rewrite the initializeSmoothModel method. The provided code 
currently implements flat shading (minus the normal calculation), and it does so using glDrawElements rather than 
glDrawArrays. glDrawElements uses an index buffer to specify the construction of primitives. The major benefit of 
glDrawElements is your mesh data can be represented more compactly by not duplicating vertices. Instead, a vertex can 
be defined a single time and its index can be referenced multiple times in an index buffer when specifying which points 
define triangles. When using flat shading, we unfortunately cannot avoid duplicating vertex data because we need to 
specify a different normal for each triangle that a point is used in, so there is no benefit to using glDrawElements. 
With smooth shading, though, any given point only needs a single combined normal of all triangles that are using it, so 
glDrawElements works great. Follow the steps below to specify normals for a smooth shaded model.

### a. Specify each vertex position only once

Change initializeSmoothModel to only append each vertex position once. You will essentially be copying the vertex position 
data from the data structure that the obj loader uses into your own positions array.

### b. Initialize normals to (0,0,0)

For every vertex position, you’ll also need a corresponding normal. Initialize your normals array with the same number of 
normals, but set them all to (0,0,0).

### c. Combine normals

Loop over every triangle in the mesh and compute its normal. Add the triangle’s normal to each of its vertex normals.

### d. Copy indices

Copy over the indices from the obj loader’s data structure, into your own indices array. 

### e. Normalize normals

After iterating over all your triangles to calculate their normals, loop over the vertex normals and normalize them. Now 
your normals are all smoothed.

## Things to notice

The only difference between smooth and flat shading is how the normal data of your mesh is specified. This means that we 
don’t need to make any changes to our shaders. By just changing the configurations of the data that we upload to our 
position and normal buffers we  can see a dramatic difference in how the model renders.

## Recommended Reading

[Phong shading](https://en.wikipedia.org/wiki/Phong_shading)
[Smooth Shading Video](https://www.youtube.com/watch?v=PMgjVJogIbc)
[glDrawElements](https://www.opengl.org/sdk/docs/man/html/glDrawElements.xhtml) - note that the indices pointer should just 
be a memory offset from the beginning of your indices VBO.
