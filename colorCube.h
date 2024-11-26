#include <iostream>
#include "GL/gl3w.h"
#include <string>
#include "Loader.h"

class ColorCube
{
private:
	GLuint vaoHandle;
	GLuint vbo_cube_vertices, vbo_cube_colors;
	GLuint ibo_cube_elements;
public:
	void setup();
	void draw();
};