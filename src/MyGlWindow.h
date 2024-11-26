#include <iostream>
#include "GL/gl3w.h"
#include <string>
#include "Loader.h"
#include "model.h"
#include "Viewer.h"
#include <glm/gtc/type_ptr.hpp>	

class MyGlWindow {
public:
	MyGlWindow(int w, int h);
	void draw();
	Viewer *viewer;

private:
	ShaderProgram* m_shaderProgram;

	GLuint vaoHandle; // vertex array object
	GLuint vbo_vertexPosition; // vbo : vertex Buffer object
	GLuint vbo_vertexColor;
	GLuint vbo_vertex;

	Model* model;

	int m_width;
	int m_height;
	void setupBuffer();
};