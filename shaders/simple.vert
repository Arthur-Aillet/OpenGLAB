#version 450

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 ModelViewPerspectiveMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ModelMatrix;

out vec4 Position;
out vec3 Normal;

void main(void)
{
	Position = ModelMatrix * vec4(vertexPosition, 1);
	Normal = NormalMatrix * vertexNormal;

	gl_Position = ModelViewPerspectiveMatrix * vec4(vertexPosition, 1.0);
}