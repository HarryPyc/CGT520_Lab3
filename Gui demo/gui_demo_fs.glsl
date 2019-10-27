#version 400

uniform sampler2D diffuse_tex;

uniform vec3 eye;
uniform float La;
uniform float Ld;
uniform float Ls;
uniform float shiness;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform bool useTexture;

out vec4 fragcolor;           
in vec2 tex_coord;

vec3 light;
vec3 ambient;
vec3 diffuse;
vec3 specular;

in vec3 dist;
in vec4 p;
in vec3 n;
in vec3 l;
vec3 r;
vec3 v;

float Rd;
float Rs;
float attenuation; 

void main(void)
{  
   vec4 tex_color = texture(diffuse_tex, tex_coord);
   Rd = max(dot(n,l), 0);

   v = normalize(eye - vec3(p));
   r = reflect(-l, n);
   Rs = pow(max(0, dot(r,v)),shiness);
   attenuation = 1/pow(length(dist),2);
   if(useTexture){
   ambient = vec3(tex_color) * La;
   diffuse = vec3(tex_color) * Rd * Ld ;
   }
   else{
   ambient = Ka * La;
   diffuse = Rd * Ld * Kd;
   }
   specular = Rs * Ls * Ks;
   light = ambient + attenuation * (diffuse + specular);
   
   fragcolor =  vec4(light,1.0);
}




















