#pragma once

#include <glm/mat4x4.hpp>
#include <vector>

void generateSphere(std::vector<glm::vec3>& verts, std::vector<glm::vec3>& norms, std::vector<uint32_t>& elements,
	float radius, uint32_t slices, uint32_t stacks);

void generateTorus(std::vector<glm::vec3>& verts, std::vector<glm::vec3>& norms, std::vector<uint32_t>& elements,
    float outerRadius, float innerRadius, int sides, int rings);

void generateTeapot(std::vector<glm::vec3>& verts, std::vector<glm::vec3>& norms, std::vector<uint32_t>& elements,
	int grid, glm::mat4 lidTransform);

void generateCube(std::vector<glm::vec3>& verts, std::vector<uint32_t>& elements, float size);

void generatePlane(std::vector<glm::vec3>& verts, std::vector<glm::vec3>& norms, std::vector<uint32_t>& elements,
	std::vector<glm::vec3>& color, int resolution, float size);

void generateBunny(std::vector<glm::vec3>& verts, std::vector<glm::vec3>& norms, std::vector<uint32_t>& elements);