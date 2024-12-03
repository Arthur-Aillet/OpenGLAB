#include "Window.hpp"

static float DEFAULT_VIEW_POINT[3] = { 14, 14, 14 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 2, 0 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };

void Window::setupBuffer()
{
	shaderProgram = new ShaderProgram();
	shaderProgram->initFromFiles("shaders/simple.vert", "shaders/simple.frag");

	shaderProgram->addUniform("Light.Position");
	shaderProgram->addUniform("Light.Ia");
	shaderProgram->addUniform("Light.Id");
	shaderProgram->addUniform("Light.Is");

	shaderProgram->addUniform("Material.Ka");
	shaderProgram->addUniform("Material.Kd");
	shaderProgram->addUniform("Material.Ks");
	shaderProgram->addUniform("Material.Shiness");

	shaderProgram->addUniform("CameraPosition");
	shaderProgram->addUniform("ModelViewPerspectiveMatrix");
	shaderProgram->addUniform("ModelMatrix");
	shaderProgram->addUniform("NormalMatrix");
}

Window::Window(int w, int h)
{
	m_width = w;
	m_height = h;
	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);
	float aspect = (w / (float)h);
	viewer = new Viewer(viewPoint, viewCenter, upVector, 45.f, aspect);
	setupBuffer();

	models.push_back(std::make_unique<Model>());
	models.push_back(std::make_unique<Model>());
	models.push_back(std::make_unique<Model>());
	models.push_back(std::make_unique<Model>());
	models.push_back(std::make_unique<Model>());

	models[0]->cow();
	models[1]->teapot(5, glm::mat4(1.f));
	models[2]->sphere(2, 50, 50);
	models[3]->torus(2, 1, 50, 50);
	models[4]->cube(2);
}

void Window::draw()
{
	glClearColor((GLfloat) 0.2,(GLfloat) 0.2,(GLfloat) 0.2, 1); //background color R G B A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); // enable depth test

	// first to parameters: starting point , next two parameters : width and height
	glViewport(0, 0, m_width, m_height); // set up the screen space

	glm::vec3 eye = viewer->getViewPoint();
	glm::vec3 look = viewer->getViewCenter();
	glm::vec3 up = viewer->getUpVector();
	glm::mat4 view = glm::lookAt(eye, look, up);
	glm::mat4 projection = glm::perspective(45.f, (float)(m_width / m_height), 0.1f, 500.f);

	glm::mat3 nmat;
	glm::mat4 mvp; //Model View Projection Matrix

	// LightInfo instance
	LightInfo light = {
		glm::vec4(12.f, 12.f, 12.f, 1.f),  // Position
		glm::vec3(0.2f, 0.2f, 0.2f),       // Ia
		glm::vec3(0.8f, 0.8f, 0.8f),       // Id
		glm::vec3(1.f, 1.f, 1.f)           // Is
	};

	// MaterialInfo instance
	MaterialInfo material = {
		glm::vec3(0.2f, 0.2f, 0.2f),       // Ka
		glm::vec3(0.8f, 0.8f, 0.2f),       // Kd
		glm::vec3(1.f, 1.f, 1.f),          // Ks
		32.f                               // Shiness
	};

	shaderProgram->use();

	// Set LightInfo uniforms
	glUniform3fv(shaderProgram->uniform("CameraPosition"), 1, glm::value_ptr(eye));
	
	glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(light.Position));
	glUniform3fv(shaderProgram->uniform("Light.Ia"), 1, glm::value_ptr(light.Ia));
	glUniform3fv(shaderProgram->uniform("Light.Id"), 1, glm::value_ptr(light.Id));
	glUniform3fv(shaderProgram->uniform("Light.Is"), 1, glm::value_ptr(light.Is));

	// Set MaterialInfo uniforms
	glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(material.Ka));
	glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(material.Kd));
	glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(material.Ks));
	glUniform1f(shaderProgram->uniform("Material.Shiness"), material.Shiness);

	for (uint16_t i = 0; i != models.size(); i++) {
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), { i * 7 - 14, 0, 0 });
		if (i == 1) { // Rotate Teapot only
			modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.f), glm::vec3(1, 0, 0));
		}
		glm::mat4 mvp = projection * view * modelMatrix; //Model View Projection Matrix
		nmat = glm::mat3(glm::transpose(glm::inverse(modelMatrix))); //Normal Matrix

		glUniformMatrix4fv(shaderProgram->uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(shaderProgram->uniform("ModelViewPerspectiveMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
		models[i]->draw();
	}

	shaderProgram->disable();
	//close shader
}