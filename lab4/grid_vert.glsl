#version 330

uniform mat4 projection;

in vec3 position;

void main() {
  gl_Position = projection * vec4(position, 1);
}
