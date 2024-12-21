#version 450 //All shaders must start with the version number

in vec4 Position;
in vec3 Normal;
in vec3 Color;
out vec4 fragColor;

struct LightInfo {
	vec4 Position; // Light position in eye coords.
	vec3 Intensity; // Ambient light intensity
};
uniform LightInfo Lights[5];

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


void main(void)
{
	vec3 finalColor = vec3(0,0,0);

	for (int i=0; i<5; i++) {
		vec3 distanceLightModel = (Lights[i].Position - Position).xyz;
		vec3 lightModel = normalize(distanceLightModel);
		vec3 V = normalize(CameraPosition - Position.xyz);
		vec3 reflect = reflect(-lightModel,Normal);
		float lambert = dot(lightModel, Normal);
		vec3 H = normalize(V + lightModel);

		float len = distanceLightModel.length();
		vec3 diffuse = Color * Material.Kd * Lights[i].Intensity * (lambert * 0.60 + 0.40) / pow(len, 2);
		vec3 specular = Material.Ks * Lights[i].Intensity * pow(max(dot(reflect,V), 0.0), Material.Shiness) / pow(len, 2);
		finalColor = finalColor + diffuse + specular;
	}

	vec3 ambient = Color * Material.Ka;
	fragColor = vec4(filmic(finalColor + ambient), 1.0);
}