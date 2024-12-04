#version 450

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;

uniform mat4 ModelViewPerspectiveMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ModelMatrix;
uniform bool HasVertexColors;

out vec4 Position;
out vec3 Normal;
out vec3 Color;

void main(void)
{
	Position = ModelMatrix * vec4(vertexPosition, 1);
	Normal = NormalMatrix * vertexNormal;
	Color = HasVertexColors ? vertexColor : vec3(1.0); 

	gl_Position = ModelViewPerspectiveMatrix * vec4(vertexPosition, 1.0);
}