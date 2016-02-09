#version 330

in vec2 position;
uniform mat4 projection;

void main() {
  gl_Position = projection * vec4(position.x, position.y, 0, 1);
}
