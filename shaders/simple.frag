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

uniform vec3 CameraPosition;
uniform mat4 ModelMatrix;

void main()
{
	vec3 distanceLightModel = (Light.Position - Position).xyz;
	float len = distanceLightModel.length();


	vec3 lightModel = normalize(distanceLightModel);
	vec3 V = normalize(CameraPosition - Position.xyz);
	vec3 reflect = reflect(-lightModel,Normal);
	vec3 ambiant = Material.Ka * Light.Ia / pow(len, 2);
	vec3 diffuse = Material.Kd * Light.Id * max(dot(lightModel, Normal), 0.0) / pow(len, 2);
	vec3 specular = Material.Ks * Light.Is * pow(max(dot(reflect,V), 0.0), Material.Shiness) / pow(len, 2);
	
	fragColor = vec4(ambiant + diffuse + specular, 1.0);
}