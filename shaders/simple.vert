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

void main(void)
{
	vec4 P = ModelMatrix * vec4(vertexPosition, 1);
	vec3 L = normalize((Light.Position - P).xyz);
	vec3 N = normalize(NormalMatrix * vertexNormal);
	vec3 V = normalize(CameraPosition - P.xyz);
	vec3 R = reflect(-L,N);
	vec3 ambiant = Material.Ka * Light.Ia;
	vec3 diffuse = Material.Kd * Light.Id * max(dot(L, N), 0.0);
	vec3 specular = Material.Ks * Light.Is * pow(max(dot(R,V), 0.0), Material.Shiness);
	LightIntensity = ambiant + diffuse + specular;

	//gl_Position = ModelViewPerspectiveMatrix * vec4(vertexPosition, 1.0);
	gl_Position = ModelViewPerspectiveMatrix * vec4(vertexPosition, 1.0);
}