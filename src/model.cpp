#include "model.h"
#include "cow.h"
#include "cube.h"
#include "shapes.h"

void Model::torus() {
	std::vector<glm::vec3> verts;
	std::vector<uint32_t> elements;

	generateTorus(verts, normals, elements, 5, 1, 50, 50);
	Model::setup(verts, elements);
}

void Model::sphere() {
	std::vector<glm::vec3> verts;
	std::vector<uint32_t> elements;

	generateSphere(verts, normals, elements, 5, 50, 50);
	Model::setup(verts, elements);
}

void Model::teapot() {
	std::vector<glm::vec3> verts;
	std::vector<uint32_t> elements;

	generateTeapot(verts, normals, elements, 5, glm::mat4(1.0f));
	Model::setup(verts, elements);
}

void Model::cube() {
	generate_normals(cow_vertices, cow_nvertices);
	Model::setup(cube_vertices, cube_nvertices);
}

void Model::cow() {
	generate_normals(cow_vertices, cow_nvertices);
	Model::setup(cow_vertices, cow_nvertices);
}

void Model::generate_normals(const std::vector<glm::vec3> &vertices, const std::vector<uint32_t> &nvertices) {
	normals.resize(vertices.size());

	for (auto i = 0; i < nvertices.size() / 3; ++i) {
		const glm::vec3& v0 = vertices[nvertices[i * 3]]; //1st vertex
		const glm::vec3& v1 = vertices[nvertices[i * 3 + 1]]; //2nd vertex
		const glm::vec3& v2 = vertices[nvertices[i * 3 + 2]]; //3rd vertex
		glm::vec3 n = glm::cross((v1 - v0), (v2 - v0)); //Cross product
		n = glm::normalize(n);
		normals[nvertices[i * 3]] += n; //Set the same normal to each vertex
		normals[nvertices[i * 3 + 1]] += n;
		normals[nvertices[i * 3 + 2]] += n;
	}

	for (auto &normal : normals) {
		normal = glm::normalize(normal);
	}
}

void Model::setup(const std::vector<glm::vec3> &vertices, const std::vector<uint32_t> &nvertices)
{
	glCreateVertexArrays(1, &vaoHandle);
	glCreateBuffers(1, &vbo_vertices);
	glCreateBuffers(1, &vbo_normals);
	glCreateBuffers(1, &ibo_elements); //Gluint ibo_cow_elements

	glNamedBufferData(vbo_vertices, vertices.size() * sizeof(float) * 3, vertices.data(), GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 0, vbo_vertices, 0, sizeof(float) * 3);

	glNamedBufferData(vbo_normals, normals.size() * sizeof(float) * 3, normals.data(), GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 1, vbo_normals, 0, sizeof(float) * 3);

	glNamedBufferData(ibo_elements, nvertices.size() * sizeof(uint32_t), nvertices.data(), GL_STATIC_DRAW);
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