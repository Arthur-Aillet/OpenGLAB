#version 450 //All shaders must start with the version number

in vec4 Position;
in vec3 Normal;
out vec4 fragColor;

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

uniform mat3 NormalMatrix;
uniform vec3 CameraPosition;
uniform mat4 ModelMatrix;

void main()
{
	vec3 LightModel = normalize((Light.Position - Position).xyz);
	vec3 V = normalize(CameraPosition - Position.xyz);
	vec3 Reflect = reflect(-LightModel,Normal);
	vec3 ambiant = Material.Ka * Light.Ia;
	vec3 diffuse = Material.Kd * Light.Id * max(dot(LightModel, Normal), 0.0);
	vec3 specular = Material.Ks * Light.Is * pow(max(dot(Reflect,V), 0.0), Material.Shiness);
	
	fragColor = vec4(ambiant + diffuse + specular, 1.0);
}