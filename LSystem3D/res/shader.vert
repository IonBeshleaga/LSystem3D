#version 330 

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texUV;

out vec3 vertColor;

uniform mat4 modelMatrix;
uniform mat4 camMatrix;
uniform mat4 projMatrix;

void main(){
	gl_Position = camMatrix*modelMatrix*vec4(position,1);
	vertColor = color;
}