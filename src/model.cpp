#include "model.hpp"
#include "cow.hpp"
#include "bunny.hpp"
#include "shapes.hpp"
#include <stdio.h>


void Model::plane(int resolution, float size) {
	std::vector<glm::vec3> d_colors;
	generatePlane(vertices, normals, elements, d_colors, resolution, size);

	colors = d_colors;
	Model::setup();
}

void Model::torus(float outerRadius, float innerRadius, int sides, int rings) {
	generateTorus(vertices, normals, elements, outerRadius, innerRadius, sides, rings);
	Model::setup();
}

void Model::sphere(float radius, uint32_t slices, uint32_t stacks) {
	generateSphere(vertices, normals, elements, radius, slices, stacks);
	Model::setup();
}

void Model::teapot(int grid, glm::mat4 lidTransform) {
	generateTeapot(vertices, normals, elements, grid, lidTransform);
	Model::setup();
}

void Model::cube(float size) {
	generateCube(vertices, elements, size);
	generate_normals();
	Model::setup();
}

void Model::bunny() {
	generateBunny(vertices, normals, elements);
	generate_normals();
	Model::setup();
}

void Model::cow() {
	vertices = cow_vertices;
	elements = cow_nvertices;
	merge_by_distance(vertices, elements, 0.1);
	generate_normals();
	Model::setup();
}

void Model::generate_normals() {
	normals.resize(vertices.size());

	for (auto i = 0; i < elements.size() / 3; ++i) {
		const glm::vec3& v0 = vertices[elements[i * 3]]; //1st vertex
		const glm::vec3& v1 = vertices[elements[i * 3 + 1]]; //2nd vertex
		const glm::vec3& v2 = vertices[elements[i * 3 + 2]]; //3rd vertex
		glm::vec3 n = glm::cross((v1 - v0), (v2 - v0)); //Cross product
		n = glm::normalize(n);
		normals[elements[i * 3]] += n; //Set the same normal to each vertex
		normals[elements[i * 3 + 1]] += n;
		normals[elements[i * 3 + 2]] += n;
	}

	for (auto &normal : normals) {
		normal = glm::normalize(normal);
	}
}

void Model::setup()
{
	glCreateVertexArrays(1, &vaoHandle);
	glCreateBuffers(1, &vbo_vertices);
	glCreateBuffers(1, &vbo_normals);
	if (colors.has_value()) {
		glCreateBuffers(1, &vbo_colors);
	}
	glCreateBuffers(1, &ibo_elements);

	glNamedBufferData(vbo_vertices, vertices.size() * sizeof(float) * 3, vertices.data(), GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 0, vbo_vertices, 0, sizeof(float) * 3);

	glNamedBufferData(vbo_normals, normals.size() * sizeof(float) * 3, normals.data(), GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 1, vbo_normals, 0, sizeof(float) * 3);

	if (colors.has_value()) {
		glNamedBufferData(vbo_colors, colors.value().size() * sizeof(float) * 3, colors.value().data(), GL_STATIC_DRAW);
		glVertexArrayVertexBuffer(vaoHandle, 2, vbo_colors, 0, sizeof(float) * 3);
	}

	glNamedBufferData(ibo_elements, elements.size() * sizeof(uint32_t), elements.data(), GL_STATIC_DRAW);
	glVertexArrayElementBuffer(vaoHandle, ibo_elements);

	glVertexArrayAttribFormat(vaoHandle, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);
	glEnableVertexArrayAttrib(vaoHandle, 0);
	
	glVertexArrayAttribFormat(vaoHandle, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);
	glEnableVertexArrayAttrib(vaoHandle, 1);

	if (colors.has_value()) {
		glVertexArrayAttribFormat(vaoHandle, 2, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexAttribBinding(2, 2);
		glEnableVertexArrayAttrib(vaoHandle, 2);
	}
}

void Model::draw()
{
	glBindVertexArray(vaoHandle);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(uint32_t), GL_UNSIGNED_INT, 0);
}