#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "Window.hpp"

bool lbutton_down;
bool rbutton_down;
bool mbutton_down;
double m_lastMouseX;
double m_lastMouseY;
double cx, cy;
Window* global_window;

void window_size_callback(GLFWwindow* _window, int width, int height)
{
    global_window->setSize(width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    cx = xpos;
    cy = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        m_lastMouseX = xpos;
        m_lastMouseY = ypos;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (GLFW_PRESS == action)
            lbutton_down = true;
        else if (GLFW_RELEASE == action)
            lbutton_down = false;
    }

    else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (GLFW_PRESS == action)
            rbutton_down = true;
        else if (GLFW_RELEASE == action)
            rbutton_down = false;
    }

    else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (GLFW_PRESS == action)
            mbutton_down = true;
        else if (GLFW_RELEASE == action)
            mbutton_down = false;
    }
}


void mouseDragging(std::shared_ptr<Window> win, double width, double height)
{
    if (lbutton_down) {
        float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
        float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
        win->viewer->rotate(fractionChangeX, fractionChangeY);
    }
    else if (mbutton_down) {
        float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
        float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
        win->viewer->zoom(fractionChangeY);
    }
    else if (rbutton_down) {
        float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
        float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
        win->viewer->translate(-fractionChangeX, -fractionChangeY, 1);
    }
    m_lastMouseX = cx;
    m_lastMouseY = cy;
}

int main() {
	/* Initialize the library */
	if (!glfwInit())
	{
		// Initialization failed
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int width = 800;
	int height = 800;

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL FrameWork", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwSwapInterval(1); //enable vsync

	glfwMakeContextCurrent(window);
	if (gl3wInit()) {
		fprintf(stderr, "failed to initialize OpenGL\n");
		return -1;
	}

	if (!gl3wIsSupported(4, 2)) {
		fprintf(stderr, "OpenGL 4.2 not supported\n");
		return -1;
	}
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
        glGetString(GL_SHADING_LANGUAGE_VERSION));
    glfwSwapInterval(1);

    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowTitle(window, "MyOpenGLWindow");


    std::shared_ptr<Window> win = std::make_shared<Window>(width, height);

    global_window = win.get();

    while (!glfwWindowShouldClose(window))
    {
        win->draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
        mouseDragging(win, width, height);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}