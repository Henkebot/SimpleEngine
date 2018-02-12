#include "window.h"

Window::Window(const char * title, int width, int height) 
	: m_Width(width), m_Height(height), m_Title(title), m_Running(true)
{
	if (!init())
		glfwTerminate();

	glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
}

bool Window::closed() const
{
	return !glfwWindowShouldClose(m_pWindow) && glfwGetKey(m_pWindow,GLFW_KEY_ESCAPE) != GLFW_PRESS;
}

void Window::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Window::update() const
{
//	glfwSetCursorPos(m_pWindow, double(m_Width) / 2.0, double(m_Height) / 2.0);
	glfwPollEvents();
	glfwSwapBuffers(m_pWindow);
}

void Window::hideAndGrabMouseMode()
{
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::restoreMouse()
{
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::getMousePosition(double & x, double & y)
{
	x = mouseX;
	y = mouseY;
}

void Window::closeWindow()
{
	glfwSetWindowShouldClose(m_pWindow, GLFW_FALSE);
}

Window::~Window()
{
	glfwDestroyWindow(m_pWindow);
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

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);


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
	
	// Set to 0 for unlimited FPS!
	glfwSwapInterval(1);

	// GLFW INIT COMPLETE FROM THIS POINT
	if (glewInit() != GLEW_OK)
	{
		DPRINT("Failed to initalize GLEW.");
		return false;
	}
	
	printf("OpenGL %s", glGetString(GL_VERSION));

	return true;
}
