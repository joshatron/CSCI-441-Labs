#version 330

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;

uniform mat4 view;

in vec3 pos;
in vec3 norm;

out vec4 color_out;

void main() {
    vec3 L;
    vec3 N = normalize(norm);

    L = normalize(lightPos-pos);

    vec3 diffuse = diffuseColor*dot(N,L);

    color_out = vec4(lightIntensity*lightColor*diffuse+ambientColor, 1);
}
