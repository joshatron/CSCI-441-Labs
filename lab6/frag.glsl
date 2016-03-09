#version 330

in vec3 fcolor;
in vec3 fposition;
in vec3 fnormal;

uniform vec3 lightPos;

out vec4 color_out;

void main() {
    vec3 normal = normalize(fnormal);
    vec3 l = normalize(lightPos - fposition);
    float diffuse = 1.0 * max(0, dot(normal, l));
    vec3 r = 2 * dot(normal, l) * normal - l;
    vec3 v = normalize(-1 * fposition);
    float specular = .5 * pow(max(0, dot(r, v)), 1);
    float ambient = .1;
    color_out = vec4(fcolor * diffuse + fcolor * ambient, 1);
}
