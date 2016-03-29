#version 330

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec3 position;
in vec3 normal;

out vec3 pos;
out vec3 norm;

void main() {
  gl_Position = projection * view * model * vec4(position, 1);
  norm = (transpose(inverse(model))*vec4(normal,0)).xyz;
  pos = (model*vec4(position,1)).xyz;
}
