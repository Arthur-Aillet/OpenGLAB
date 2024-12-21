#pragma once

#include <iostream>
#include "GL/gl3w.h"
#include <string>
#include "Loader.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <optional>
#include <algorithm>
#include <limits>

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

	template <typename... T>
	static void merge_by_distance(std::vector<glm::vec3>& vertices, std::vector<uint32_t>& indices, float merge_distance, std::vector<T>&... cleanups);
	template <typename... T>
	static void trim_geometry(std::vector<glm::vec3>& vertices, std::vector<uint32_t>& indices, std::vector<T>&... cleanups);

public:
	std::optional<std::vector<glm::vec3>> colors;
	Material material;

	void obj(std::string path);
	void draw();
	void cow();
	void bunny();
	void cube(float size);
	void torus(float outerRadius, float innerRadius, int sides, int rings);
	void sphere(float radius, uint32_t slices, uint32_t stacks);
	void teapot(int grid, glm::mat4 lidTransform);
	void plane(int resolution, float size);
};

//Julian Scott
template <typename... T>
void Model::trim_geometry(std::vector<glm::vec3>& vertices, std::vector<uint32_t>& indices, std::vector<T>&... cleanups) {
	std::vector<bool> unused; //[uint32_t];
	unused.resize(vertices.size(), true);

	std::for_each(indices.begin(), indices.end(), [&vertices, &unused, &indices](auto idx) {unused[idx] = false; });

	size_t offset = 0;
	std::vector<size_t> offsets = std::vector<size_t>(vertices.size(), 0);

	std::vector<glm::vec3> new_vertices;
	new_vertices.reserve(vertices.size());

	for (uint32_t i = 0; i < vertices.size(); ++i) {
		offsets[i] = offset;
		if (unused[i]) {
			offset += 1;
		}
		else {
			new_vertices.push_back(vertices[i]);
			((cleanups[i - offset] = cleanups[i]), ...);
		}
	}

	std::vector<uint32_t> new_indices;

	new_indices.resize(indices.size());

	for (size_t i = 0; i < new_indices.size(); ++i) {
		//if (!unused[indices[i]])
		new_indices[i] = (indices[i] - offsets[indices[i]]);
	}

	indices = new_indices;
	vertices = new_vertices;
}

template <typename... T>
void Model::merge_by_distance(std::vector<glm::vec3>& vertices, std::vector<uint32_t>& indices, float merge_distance, std::vector<T>&... cleanups) {
	std::vector<std::vector<float>> distances = std::vector<std::vector<float>>(vertices.size(), std::vector<float>(vertices.size(), 0.));
	// [uint32_t][uint32_t]


	// /*
	for (size_t i = 0; i < vertices.size(); ++i) {
		for (size_t j = 0; j < vertices.size(); ++j) {
			distances[i][j] = glm::length(vertices[i] - vertices[j]);
		}
	}

	std::vector<uint32_t> redirect; //[uint32_t]
	redirect.resize(vertices.size());


	for (uint32_t i = 0; i < vertices.size(); ++i) {
		redirect[i] = i;
		float best_dist = std::numeric_limits<float>::infinity();
		for (uint32_t j = 0; j < vertices.size(); ++j) {
			float distance = distances[i][j];
			if (distance < merge_distance && distance < best_dist && i != j && redirect[j] == j) {
				best_dist = distance;
				redirect[i] = j;
				//std::cout << "merging " << i << " with " << j << " at distance " << distance << std::endl;
				break;
			}
		}
	}

	for (size_t i = 0; i < indices.size(); ++i) {
		indices[i] = redirect[indices[i]];
	}
	// */

	Model::trim_geometry(vertices, indices, cleanups...);
}
