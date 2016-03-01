#version 330

in vec3 fcolor;
out vec4 color_out;

void main() {
  color_out = vec4(fcolor, 1);
}
