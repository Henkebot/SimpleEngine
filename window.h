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
	GLFWwindow* m_pWindow;
public:
	Window(const char* title, int width, int height);
	
	bool closed() const;
	
	void clear() const;
	void update() const;
	virtual ~Window();
private:
	bool init();
	
};
// Not possible in class because of "this pointer"
void resize_callback(GLFWwindow* window, int width, int height);

#endif