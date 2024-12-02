#include <iostream>
#include "GL/gl3w.h"
#include <string>
#include "Loader.h"
#include "model.h"
#include "Viewer.h"
#include <glm/gtc/type_ptr.hpp>	

struct LightInfo {
	glm::vec4 Position; // Light position in eye coords.
	glm::vec3 Ia; // Ambient light intensity
	glm::vec3 Id; // Diffuse light intensity
	glm::vec3 Is; // Specular light intensity
};

struct MaterialInfo {
	glm::vec3 Ka; // Ambient reflectivity
	glm::vec3 Kd; // Diffuse reflectivity
	glm::vec3 Ks; // Specular reflectivity
	float Shiness; // Specular shininess factor (phong exponent)
};

class MyGlWindow {
public:
	MyGlWindow(int w, int h);
	void draw();
	Viewer *viewer;

private:
	ShaderProgram* shaderProgram;

	GLuint vaoHandle; // vertex array object
	GLuint vbo_vertexPosition; // vbo : vertex Buffer object
	GLuint vbo_vertexColor;
	GLuint vbo_vertex;

	std::vector<std::unique_ptr<Model>> models;

	int m_width;
	int m_height;
	void setupBuffer();
};