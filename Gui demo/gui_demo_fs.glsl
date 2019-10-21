#version 400

uniform sampler2D diffuse_tex;
uniform vec3 lightcolor;
uniform float Ka;
uniform float Kd;
uniform float Ks;

out vec4 fragcolor;           
in vec2 tex_coord;

vec3 light;
vec3 ambient;
vec3 diffuse;
vec3 specular;

in float Rd;
in float Rs;
in float attenuation; 

void main(void)
{  
   
   ambient = lightcolor * Ka;
   diffuse = Rd * Kd * lightcolor;
   specular = Rs * Ks * lightcolor;
   light = ambient + attenuation * (diffuse + specular);
   vec4 tex_color = texture(diffuse_tex, tex_coord);
   fragcolor =  tex_color * vec4(light,1.0);
}




















