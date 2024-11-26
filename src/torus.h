#ifndef TORUS_H
#define TORUS_H

#include <glm/mat4x4.hpp>
#include <vector>

void generateTorusVerts(std::vector<glm::vec3>& verts, std::vector<glm::vec3>& norms, std::vector<uint32_t>& elements,
    float outerRadius, float innerRadius, int sides, int rings);

#endif // TORUS_H