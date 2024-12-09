#pragma once

#include <iostream>
#include "GL/gl3w.h"
#include <string>
#include "Loader.hpp"
#include "Model.hpp"
#include "Viewer.hpp"
#include <glm/gtc/type_ptr.hpp>	

struct LightInfo {
	glm::vec4 Position; // Light position in eye coords.
	glm::vec3 Intensity; // Ambient light intensity
};

class Window {
public:
	Window(int w, int h);
	void draw();
	Viewer *viewer;
	void setSize(int w, int h);

private:
	ShaderProgram* shaderProgram;

	GLuint vaoHandle; // vertex array object
	GLuint vbo_vertexPosition; // vbo : vertex Buffer object
	GLuint vbo_vertexColor;
	GLuint vbo_vertex;

	std::vector<std::unique_ptr<Model>> models;

	int width;
	int height;
	void setupBuffer();
};