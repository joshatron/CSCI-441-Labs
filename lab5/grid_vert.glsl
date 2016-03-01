#version 330

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 rotation;

in vec3 position;

void main() {
  gl_Position = projection * rotation * view * model * vec4(position, 1);
}
