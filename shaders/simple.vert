#version 450

// in vec3 coord3d;
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 mvp;
uniform vec4 LightLocation;
uniform vec3 Kd;
uniform vec3 Ld;
uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
out vec3 LightIntensity;

void main(void)
{
	vec3 N = normalize(NormalMatrix * vertexNormal);

	vec4 P = ModelViewMatrix * vec4(vertexPosition, 1); //How to transform the VertexPosition into Eye Coordinate?
	vec4 L = normalize((ModelViewMatrix * LightLocation) - P); //Vector from Light Locaiton to P
	LightIntensity = Kd * Ld * max(dot(L.xyz, N), 0); //Calculate Kd*Id*(L·N) //Remember that if (L·N) < 0, set it to 0.
	gl_Position = mvp * vec4(vertexPosition, 1.0);
}