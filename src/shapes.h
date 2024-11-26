#ifndef SHAPES_H
#define SHAPES_H

#include <glm/mat4x4.hpp>
#include <vector>

void generateSphere(std::vector<glm::vec3>& verts, std::vector<glm::vec3>& norms, std::vector<uint32_t>& elements,
	float radius, uint32_t slices, uint32_t stacks);

void generateTorus(std::vector<glm::vec3>& verts, std::vector<glm::vec3>& norms, std::vector<uint32_t>& elements,
    float outerRadius, float innerRadius, int sides, int rings);

void generateTeapot(std::vector<glm::vec3>& verts, std::vector<glm::vec3>& norms, std::vector<uint32_t>& elements,
	int grid, glm::mat4 lidTransform);


#endif // SHAPES_H