#version 330

uniform mat4 projection;
uniform mat4 view;

in vec3 position;

void main() {
  gl_Position = projection * (view * vec4(position, 1));
}
