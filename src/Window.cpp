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

	shaderProgram->addUniform("HasVertexColors");
}

void Window::setSize(int w, int h) {
	float aspect = (w / (float)h);

	width = w;
	height = h;
	
	viewer->setAspectRatio(aspect);
}

Window::Window(int w, int h)
{
	width = w;
	height = h;
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
	models.push_back(std::make_unique<Model>());

	models[0]->cow();
	models[0]->material = Material(glm::vec3(0.04f, 0.8f, 0.04f), glm::vec3(0.1f, 0.8f, 0.8f), glm::vec3(1.f), 32.f);
	models[1]->teapot(15, glm::mat4(1.f));
	models[2]->sphere(2, 50, 50);
	models[2]->material = Material(glm::vec3(0.04f, 0.04f, 0.8f), glm::vec3(0.8f, 0.05f, 0.8f), glm::vec3(1.f), 32.f);
	models[3]->torus(2, 1, 50, 50);
	models[3]->material = Material(glm::vec3(0.8f, 0.1f, 0.8f), glm::vec3(0.03f, 0.8f, 0.03f), glm::vec3(1.f), 32.f);
	models[4]->cube(2);
	models[4]->material = Material(glm::vec3(0.8f, 0.04f, 0.04f), glm::vec3(0.1f, 0.1f, 0.8f), glm::vec3(1.f), 32.f);
	models[5]->plane(100, 300);
	models[5]->material = Material(glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f), 32.f);
}

void Window::draw()
{
	glClearColor((GLfloat) 0.2,(GLfloat) 0.2,(GLfloat) 0.2, 1); //background color R G B A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); // enable depth test

	// first to parameters: starting point , next two parameters : width and height
	glViewport(0, 0, width, height); // set up the screen space

	glm::vec3 eye = viewer->getViewPoint();
	glm::vec3 look = viewer->getViewCenter();
	glm::vec3 up = viewer->getUpVector();
	glm::mat4 view = glm::lookAt(eye, look, up);
	glm::mat4 projection = glm::perspective(45.f, (float)(width) / height, 0.1f, 500.f);

	// LightInfo instance
	LightInfo light = {
		glm::vec4(10.f, 10.f, 10.f, 1.f),   // Position
		glm::vec3(0.08f, 0.08f, 0.08f) * 5.f, // Ia
		glm::vec3(0.8f, 0.8f, 0.8f) * 5.f, // Id
		glm::vec3(1.f, 1.f, 1.f) * 5.f     // Is
	};

	shaderProgram->use();

	// Set LightInfo uniforms
	glUniform3fv(shaderProgram->uniform("CameraPosition"), 1, glm::value_ptr(eye));
	
	glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(light.Position));
	glUniform3fv(shaderProgram->uniform("Light.Ia"), 1, glm::value_ptr(light.Ia));
	glUniform3fv(shaderProgram->uniform("Light.Id"), 1, glm::value_ptr(light.Id));
	glUniform3fv(shaderProgram->uniform("Light.Is"), 1, glm::value_ptr(light.Is));


	for (uint16_t i = 0; i != models.size(); i++) {
		// Set MaterialInfo uniforms
		Material material = models[i].get()->material;
		glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(material.Ka));
		glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(material.Kd));
		glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(material.Ks));
		glUniform1f(shaderProgram->uniform("Material.Shiness"), material.Shiness);

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.), { i * 7. - 14., 0., 0. });
		if (i == 1) { // Rotate Teapot only
			modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.f), glm::vec3(1, 0, 0));
		}
		if (i == 5) {
			modelMatrix = glm::mat4(1.);
		}
		if (i == 4) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0., 1., 0.));
		}
		if (i == 3) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0., 1., 0.));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.f), glm::vec3(1, 0, 0));
		}
		if (i == 2) {
			modelMatrix = glm::translate(glm::mat4(1.), { 0., 2, 0. });
		}
		glm::mat4 mvp = projection * view * modelMatrix; //Model View Projection Matrix
		glm::mat3 nmat = glm::mat3(glm::transpose(glm::inverse(modelMatrix))); //Normal Matrix

		bool useColor = models[i]->colors.has_value();
		glUniform1i(shaderProgram->uniform("HasVertexColors"), useColor);
		glUniformMatrix4fv(shaderProgram->uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(shaderProgram->uniform("ModelViewPerspectiveMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
		models[i]->draw();
	}

	shaderProgram->disable();
}