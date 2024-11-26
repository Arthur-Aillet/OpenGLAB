#include <iostream>
#include "GL/gl3w.h"
#include <string>
#include "Loader.h"
#include <glm/glm.hpp>
#include <vector>

class Model
{
private:
	GLuint vaoHandle;
	GLuint vbo_vertices, vbo_normals;
	GLuint ibo_elements;
	std::vector<glm::vec3> normals;
	void setup(const std::vector<glm::vec3> &v, const std::vector<uint32_t>& e);
	void generate_normals(const std::vector<glm::vec3>& v, const std::vector<uint32_t>& e);
public:
	void draw();
	void cow();
	void cube();
	void torus();
	void sphere();
	void teapot();
};