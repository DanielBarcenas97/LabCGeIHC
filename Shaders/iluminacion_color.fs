#version 330 core

in vec3 our_normal;
in vec3 fragPos;

struct LightPosition(){
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform LightPosition Light;
uniform vec4 ourColor; 
out vec4 color;

void main()
{
	vec3 ambient = Light.ambient * vec3(ourColor);
    color = vec4(ambient,1.0);

}
