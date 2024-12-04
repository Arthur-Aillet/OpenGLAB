#pragma once
#include <glm/glm.hpp>

class Material {
public:
	glm::vec3 Ka = glm::vec3(0.8f, 0.2f, 0.2f); // Ambient reflectivity
	glm::vec3 Kd = glm::vec3(0.9f, 0.9f, 0.1f); // Diffuse reflectivity
	glm::vec3 Ks = glm::vec3(1.f, 1.f, 1.f); // Specular reflectivity
	float Shiness = 32.f; // Specular shininess factor (phong exponent)
	Material(glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float Shiness);
	Material();
};
