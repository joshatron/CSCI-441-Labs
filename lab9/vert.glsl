#version 330

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec3 position;
in vec3 color;
in vec2 uv;

out vec3 fcolor;
out vec2 fuv;

void main() {
  gl_Position = projection * view * model * vec4(position, 1);
  fcolor = color;
  fuv = uv;
}
