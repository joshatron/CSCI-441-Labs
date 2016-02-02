#version 330

in vec2 position;
in vec3 color_in;
out vec4 color;

void main() {
  gl_Position = vec4(position.x, position.y, 0, 1);
  color = vec4(color_in.r, color_in.g, color_in.b, 1.0);
}
