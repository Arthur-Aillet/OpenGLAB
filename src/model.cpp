#include "model.h"
#include "cow.h"
#include "cube.h"
#include "shapes.h"

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

void Model::cube() {
	vertices = cube_vertices;
	elements = cube_nvertices;
	generate_normals();
	Model::setup();
}

void Model::cow() {
	vertices = cow_vertices;
	elements = cow_nvertices;
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
	glCreateBuffers(1, &ibo_elements); //Gluint ibo_cow_elements

	glNamedBufferData(vbo_vertices, vertices.size() * sizeof(float) * 3, vertices.data(), GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 0, vbo_vertices, 0, sizeof(float) * 3);

	glNamedBufferData(vbo_normals, normals.size() * sizeof(float) * 3, normals.data(), GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 1, vbo_normals, 0, sizeof(float) * 3);

	glNamedBufferData(ibo_elements, elements.size() * sizeof(uint32_t), elements.data(), GL_STATIC_DRAW);
	glVertexArrayElementBuffer(vaoHandle, ibo_elements);

	glVertexArrayAttribFormat(vaoHandle, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);
	glEnableVertexArrayAttrib(vaoHandle, 0);
	
	glVertexArrayAttribFormat(vaoHandle, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);
	glEnableVertexArrayAttrib(vaoHandle, 1);
}

void Model::draw()
{
	glBindVertexArray(vaoHandle);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(uint32_t), GL_UNSIGNED_INT, 0);
}