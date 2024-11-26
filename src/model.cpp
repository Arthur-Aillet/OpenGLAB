#include "model.h"

void Model::setup() //Call from constructor
{
	normals.resize(1732);

	for (auto i = 0; i < 9468 / 3; ++i) {
		const glm::vec3& v0 = vertices[nvertices[i * 3]]; //1st vertex
		const glm::vec3& v1 = vertices[nvertices[i * 3 + 1]]; //2nd vertex
		const glm::vec3& v2 = vertices[nvertices[i * 3 + 2]]; //3rd vertex
		glm::vec3 n = glm::cross((v1 - v0), (v2 - v0)); //Cross product
		n = glm::normalize(n);
		normals[nvertices[i * 3]] += n; //Set the same normal to each vertex
		normals[nvertices[i * 3 + 1]] += n;
		normals[nvertices[i * 3 + 2]] += n;
	}

	for (auto i = 0; i < normals.size(); ++i) {
		normals[i] = glm::normalize(normals[i]);
	}

	GLuint ibo_cow_elements;

	glCreateVertexArrays(1, &vaoHandle);
	glCreateBuffers(1, &vbo_vertices);
	glCreateBuffers(1, &vbo_normals);
	glCreateBuffers(1, &ibo_elements); //Gluint ibo_cow_elements

	glNamedBufferData(vbo_vertices, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 0, vbo_vertices, 0, sizeof(float) * 3);

	glNamedBufferData(vbo_normals, normals.size() * sizeof(float) * 3, normals.data(), GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 1, vbo_normals, 0, sizeof(float) * 3);

	glNamedBufferData(ibo_elements, sizeof(nvertices), nvertices, GL_STATIC_DRAW);
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
	int size = 1732;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(uint32_t), GL_UNSIGNED_INT, 0);
}