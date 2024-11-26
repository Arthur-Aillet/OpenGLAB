#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>
#include <vector>

static const std::vector<glm::vec3> cube_vertices = {
	// front
	{ -1.0, -1.0, 1.0 },
	{ 1.0, -1.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ -1.0, 1.0, 1.0 },
	// back
	{ -1.0, -1.0, -1.0 },
	{ 1.0, -1.0, -1.0 },
	{ 1.0, 1.0, -1.0 },
	{ -1.0, 1.0, -1.0 },
};

static const std::vector<uint32_t> cube_nvertices = {
	0, 1, 2, 2, 3, 0, 1, 5, 6,
	6, 2, 1, 7, 6, 5, 5, 4, 7,
	4, 0, 3, 3, 7, 4, 4, 5, 1,
	1, 0, 4, 3, 2, 6, 6, 7, 3,
};

#endif // CUBE_H
