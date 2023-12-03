#version 410

uniform samplerCube sky;

in data {
  vec3 normal;
  vec3 view;
} f;

out vec4 color;

void main (void) 
{
    vec3 normal = normalize(f.normal);
    vec3 view = normalize(f.view);
    vec3 dir = reflect (-view, normal) * vec3 (1, 1, -1);
    color = 0.9f * texture (sky, dir);
}