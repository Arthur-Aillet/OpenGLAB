#version 450 //All shaders must start with the version number

in vec4 Position;
in vec3 Normal;
in vec3 Color;
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


vec3 filmic(vec3 x) {
	vec3 X = max(vec3(0.0), x - 0.004);
	vec3 result = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);
	return result;
	//return pow(result, vec3(2.2));
}


void main()
{
	vec3 distanceLightModel = (Light.Position - Position).xyz;
	float len = distanceLightModel.length();


	vec3 lightModel = normalize(distanceLightModel);
	vec3 V = normalize(CameraPosition - Position.xyz);
	vec3 reflect = reflect(-lightModel,Normal);
	vec3 ambiant = Color * Material.Ka * Light.Ia / pow(len, 2);
	float lambert = dot(lightModel, Normal);
	vec3 diffuse = Color * Material.Kd * Light.Id * (lambert * 0.5 + 0.5) / pow(len, 2);
	vec3 H = normalize(V + lightModel);
	// vec3 specular = Light.Is * Material.Ks * pow(max(dot(H,Normal), 0.0), Material.Shiness * 2) * max(lambert, 0);
	vec3 specular = Material.Ks * Light.Is * pow(max(dot(reflect,V), 0.0), Material.Shiness) / pow(len, 2);
	
	fragColor = vec4(filmic(ambiant + diffuse + specular), 1.0);
}