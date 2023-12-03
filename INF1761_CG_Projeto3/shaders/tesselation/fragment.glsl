#version 410

uniform vec4 lamb;
uniform vec4 ldif;
uniform vec4 lspe;
uniform samplerCube sky;
uniform vec4 mamb;
uniform vec4 mdif;
uniform vec4 mspe;
uniform float mshi;
uniform float mopacity;
uniform float fdensity;
uniform vec4 fcolor;
uniform sampler2D decal;
uniform vec4 cpos;
in data {
  vec3 normal;
  vec3 view;
  vec3 light;
  vec2 texcoord;
  vec4 pos;
} f;

out vec4 color;

void main (void) 
{
  vec3 normal = normalize(f.normal);
  vec3 view = normalize(-f.pos.xyz);
  vec3 light = normalize(f.light);
  float ndotl = dot(normal, light);
  
  color = mamb * lamb + mdif * ldif * max(0, ndotl) * texture(decal, f.texcoord); 

  if (ndotl > 0) {
    vec3 refl = normalize(reflect(-light, normal));
    color += mspe * lspe * pow(max(0, dot(refl, view)), mshi); 
  }

  color.a = mopacity;

  // Fog calculation
    color = texture(sky, f.pos.xyz);
    float fog = exp(-pow(fdensity * distance(f.pos,cpos), 2));
    color = fog * color + (1.0 - fog) * fcolor;

  
}