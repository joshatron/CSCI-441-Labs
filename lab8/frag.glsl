#version 330

// Part 1 - Add the variable that you output from your vertex shader. Assign your uv coordinate to the red and green components of color_out. This is a great
// way to debug your texture coordinates. Each face of your cube should have a black corner (0,0), a red corner (1,0), a green corner (0,1) and a yellow 
// corner (1,1). 

// Part 2 e - Add a uniform sampler2D variable for you texture. Sample the texture using the texture function (vec4 color = texture(<tex variable>, <uv coordinate>)).

in vec3 fcolor;
out vec4 color_out;

void main() {
  color_out = vec4(fcolor, 1);
}
