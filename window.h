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

#define main int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) 

class Window
{
private:
	int m_Width;
	int m_Height;

	const char* m_Title;

	double mouseX;
	double mouseY;

	GLFWwindow* m_pWindow;
	bool m_Running;
public:
	Window(const char* title, int width, int height);
	virtual ~Window();
	
	bool closed() const;
	
	void clear() const;
	void update() const;

	void hideAndGrabMouseMode();
	void restoreMouse();

	void getMousePosition(double& x, double& y);
	void closeWindow();

	inline const int getWidth() const { return m_Width; }
	inline const int getHeight() const { return m_Height; }

private:
	bool init();
	friend static void resize_callback(GLFWwindow* window, int width, int height);
	friend static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos);
	
};

#endif