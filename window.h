#ifndef WINODW_H
#define WINDOW_H
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Windows.h>
#if _DEBUG
#include <iostream>
#define DPRINT(text) std::cout << (text)
#elif
#define DPRINT(text)
#endif

class Window
{
private:
	int m_Width;
	int m_Height;

	const char* m_Title;

	double mouseX;
	double mouseY;

	GLFWwindow* m_pWindow;
public:
	Window(const char* title, int width, int height);
	
	bool closed() const;
	
	void clear() const;
	void update() const;
	void getMousePosition(double& x, double& y);
	virtual ~Window();
private:
	bool init();
	friend static void resize_callback(GLFWwindow* window, int width, int height);
	friend static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos);
	
};
// Not possible in class because of "this pointer"


#endif