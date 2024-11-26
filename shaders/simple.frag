#version 450 //All shaders must start with the version number

in vec3 LightIntensity;
out vec4 fragColor;
void main()
{
	fragColor = vec4(LightIntensity, 1.0);
}