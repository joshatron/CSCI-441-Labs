#version 330

// Part 3 - Add a trackball matrix variable which is continuously updated
// as you click and drag. Multiply it into the chain of transformations 
// below.

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 rotation;

in vec3 position;
in vec3 color;
out vec3 fcolor;

void main() {
  gl_Position = projection * rotation * view * model * vec4(position, 1);
  fcolor = color;
}
