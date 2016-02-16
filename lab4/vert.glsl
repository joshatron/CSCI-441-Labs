#version 330

uniform mat4 projection;

in vec3 position;
in vec3 color;
out vec3 fcolor;

void main() {
  gl_Position = projection * vec4(position, 1);
  fcolor = color;
}
