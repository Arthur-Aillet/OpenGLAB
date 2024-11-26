#include "MyGlWindow.h"

static float DEFAULT_VIEW_POINT[3] = { 14, 14, 14 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 6, 0 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };

void MyGlWindow::setupBuffer()
{
	m_shaderProgram = new ShaderProgram();
	m_shaderProgram->initFromFiles("shaders/simple.vert", "shaders/simple.frag");
	m_shaderProgram->addUniform("mvp");
	m_shaderProgram->addUniform("LightLocation"); //Light Position : vec4
	m_shaderProgram->addUniform("Kd"); //Diffuse Object Color :vec3
	m_shaderProgram->addUniform("Ld"); //Diffuse Light Color : vec3
	m_shaderProgram->addUniform("ModelViewMatrix"); //View*Model : mat4
	m_shaderProgram->addUniform("NormalMatrix"); //Refer next slide : mat3
}

MyGlWindow::MyGlWindow(int w, int h)
{
	m_width = w;
	m_height = h;
	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);
	float aspect = (w / (float)h);
	viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);
	setupBuffer();

	model = new Model();
	model->torus();
}

void MyGlWindow::draw()
{
	glClearColor((GLfloat) 0.2,(GLfloat) 0.2,(GLfloat) 0.2, 1); //background color R G B A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); // enable depth test

	// first to parameters: starting point , next two parameters : width and height
	glViewport(0, 0, m_width, m_height); // set up the screen space

	glm::mat4 modelMatrix = glm::mat4(1.0);
	glm::vec3 eye = viewer->getViewPoint();
	glm::vec3 look = viewer->getViewCenter();
	glm::vec3 up = viewer->getUpVector();
	glm::mat4 view = glm::lookAt(eye, look, up);
	glm::mat4 projection = glm::perspective(45.0f, (float)(m_width / m_height), 0.1f, 500.0f);

	glm::mat4 mvp = projection * view * modelMatrix; //Model View Projection Matrix
	glm::vec4 lightPos(50, 50, 50, 1); //light position
	glm::vec3 Kd(1, 1, 0); //Diffuse Object Color
	glm::vec3 Ld(1, 1, 1); //Diffuse Light Color
	glm::mat4 imvp = glm::inverse(view);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp)); //Normal Matrix
	glm::mat4 mvmat = view * modelMatrix; //Model View Matrix

	//draw something
	m_shaderProgram->use();

	glUniformMatrix4fv(m_shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4fv(m_shaderProgram->uniform("LightLocation"), 1, glm::value_ptr(lightPos));
	glUniform3fv(m_shaderProgram->uniform("Kd"), 1, glm::value_ptr(Kd));
	glUniform3fv(m_shaderProgram->uniform("Ld"), 1, glm::value_ptr(Ld));
	glUniformMatrix3fv(m_shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
	glUniformMatrix4fv(m_shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mvmat));
	if (model) {
		model->draw();
	}
	/*if (cube) {
		cube->draw();
	}*/

	m_shaderProgram->disable();
	//close shader
}