#version 330

uniform sampler2D tex;

in vec3 fcolor;
in vec2 fuv;

out vec4 color_out;

void main() {
  color_out = texture(tex,fuv).rrra;
//  color_out = vec4(fuv,0,1);
}
