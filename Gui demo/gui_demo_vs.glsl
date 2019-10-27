#version 400            
uniform mat4 PVM;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;


uniform vec3 lightpos;

uniform float time;

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;

out vec3 dist;
out vec4 p;
out vec3 n;
out vec3 l;




out float attenuation;
out vec2 tex_coord;  

void main(void)
{
   gl_Position = P*V*M*vec4(pos_attrib, 1.0);
   p =  M*vec4(pos_attrib,1.0);
   dist = vec3(vec4(lightpos,1.0) - p);

   l = normalize(dist);
   n = vec3(normalize(M * vec4(normal_attrib, 0.0)));
   
   
 
   tex_coord = tex_coord_attrib;
}