#version 450

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

struct LightInfo {
	vec4 Position; // Light position in eye coords.
	vec3 Ia; // Ambient light intensity
	vec3 Id; // Diffuse light intensity
	vec3 Is; // Specular light intensity
};
uniform LightInfo Light;

struct MaterialInfo {
	vec3 Ka; // Ambient reflectivity
	vec3 Kd; // Diffuse reflectivity
	vec3 Ks; // Specular reflectivity
	float Shiness; // Specular shininess factor (phong exponent)
};
uniform MaterialInfo Material;

uniform mat4 ModelViewPerspectiveMatrix;
uniform mat3 NormalMatrix;
uniform vec3 CameraPosition;
uniform mat4 ModelMatrix;
out vec3 LightIntensity;

out vec4 Position;
out vec3 Normal;

void main(void)
{
	Position = ModelMatrix * vec4(vertexPosition, 1);
	Normal = NormalMatrix * vertexNormal;

	gl_Position = ModelViewPerspectiveMatrix * vec4(vertexPosition, 1.0);
}