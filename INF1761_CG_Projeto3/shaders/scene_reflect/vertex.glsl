#version 410

layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 normal;

uniform vec4 cpos;  // camera pos in lighting space
uniform mat4 Mv; 
uniform mat4 Mn; 
uniform mat4 Mvp;

out data {
  vec3 normal;
  vec3 view;
} v;

void main (void) 
{
  vec3 p = vec3(Mv*pos);
  v.view = normalize(vec3(cpos)-p);
  v.normal = normalize(vec3(Mn*vec4(normal,0.0f)));
  gl_Position = Mvp*pos; 
}