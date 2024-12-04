#pragma once
#include <glm/glm.hpp>

class Material {
public:
	glm::vec3 Ka = glm::vec3(0.85f, 0.05f, 0.04f); // Ambient reflectivity
	glm::vec3 Kd = glm::vec3(0.85f, 0.85f, 0.01f); // Diffuse reflectivity
	glm::vec3 Ks = glm::vec3(1.f, 1.f, 1.f); // Specular reflectivity
	float Shiness = 32.f; // Specular shininess factor (phong exponent)
	Material(glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float Shiness);
	Material();
};
