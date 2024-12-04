#version 450

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 ModelMatrix;
uniform mat4 ModelViewPerspectiveMatrix;

out vec4 Position;
out vec4 Normal;

void main(void)
{
	Position = ModelMatrix * vec4(vertexPosition, 1);
	Normal = ModelMatrix * vec4(vertexNormal, 1);

	gl_Position = ModelViewPerspectiveMatrix * vec4(vertexPosition, 1.0);
}