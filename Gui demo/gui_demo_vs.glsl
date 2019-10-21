#version 400            
uniform mat4 PVM;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform float shiness;
uniform vec3 lightpos;
uniform vec3 eye;
uniform float time;

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;

vec3 dist;
vec4 p;
vec3 n;
vec3 l;
vec3 r;
vec3 v;


out float Rd;
out float Rs;
out float attenuation;
out vec2 tex_coord;  

void main(void)
{
   gl_Position = P*V*M*vec4(pos_attrib, 1.0);
   p =  M*vec4(pos_attrib,1.0);
   dist = vec3(vec4(lightpos,1.0) - p);

   l = normalize(dist);
   n = vec3(normalize(M * vec4(normal_attrib, 1.0)));
   Rd = max(dot(n,l), 0);

   v = normalize(eye - vec3(p));
   r = reflect(-l, n);
   Rs = pow(max(0, dot(r,v)),shiness);
   attenuation = 1/pow(length(dist),2);
   tex_coord = tex_coord_attrib;
}