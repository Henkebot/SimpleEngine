#include "window.h"

Window::Window(const char * title, int width, int height) 
	: m_Width(width), m_Height(height), m_Title(title)
{
	if (!init())
		glfwTerminate();

	glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
}

bool Window::closed() const
{
	return !glfwWindowShouldClose(m_pWindow);
}

void Window::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::update() const
{
	glfwPollEvents();
	glfwSwapBuffers(m_pWindow);
}

void Window::getMousePosition(double & x, double & y)
{
	x = mouseX;
	y = mouseY;
}

Window::~Window()
{
	//glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void resize_callback(GLFWwindow * window, int width, int height) 
{
	glViewport(0, 0, width, height);
}

void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	
	win->mouseX = xPos;
	win->mouseY = yPos;

}

bool Window::init()
{
	// Create a console
#if _DEBUG
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
#endif

	if (!glfwInit())
	{
		DPRINT("Failed to initalize GLFW.");
		return false;
	}

	m_pWindow = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);

	if (!m_pWindow)
	{
		DPRINT("Failed to Create GLFW Window.");
		return false;
	}

	glfwMakeContextCurrent(m_pWindow);
	glfwSetWindowUserPointer(m_pWindow, this);
	glfwSetWindowSizeCallback(m_pWindow, resize_callback);
	glfwSetCursorPosCallback(m_pWindow, cursor_position_callback);

	// GLFW INIT COMPLETE FROM THIS POINT
	if (glewInit() != GLEW_OK)
	{
		DPRINT("Failed to initalize GLEW.");
		return false;
	}
	
	printf("OpenGL %s", glGetString(GL_VERSION));

	return true;
}
