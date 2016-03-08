#version 330

in vec3 fcolor;
in vec3 fposition;
in vec3 fnormal;

uniform vec3 lightPos;

out vec4 color_out;

void main() {
    vec3 l = normalize(lightPos - fposition);
    float diffuse = .5 * max(0, dot(fnormal, l));
    vec3 r = 2 * dot(fnormal, l) * fnormal - l;
    vec3 v = normalize(-1 * fposition);
    float specular = .5 * power(max(0, dot(r, v)), 1);
    float ambient = .1;
    color_out = vec4(fcolor * diffuse + vec3(specular) + fcolor * ambient, 1);
}
