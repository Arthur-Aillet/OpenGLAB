#pragma once

#include <iostream>
#include "GL/gl3w.h"
#include <string>
#include "Loader.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <optional>

#include "Material.hpp"

class Model
{
private:
	GLuint vaoHandle;
	GLuint vbo_vertices, vbo_normals, vbo_colors;
	GLuint ibo_elements;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<uint32_t> elements;

	void setup();
	void generate_normals();
public:
	std::optional<std::vector<glm::vec3>> colors;
	Material material;

	void draw();
	void cow();
	void cube(float size);
	void torus(float outerRadius, float innerRadius, int sides, int rings);
	void sphere(float radius, uint32_t slices, uint32_t stacks);
	void teapot(int grid, glm::mat4 lidTransform);
	void plane(int resolution, float size);
};