#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
class Window {
public:
	GLFWwindow* window;
	int width, height;
	const char* title;
public:
	Window(int width, int height, const char* title);
	~Window();
	bool isShouldClose();
	void swapBuffers();
	void pollEvents();
	glm::vec2 getSize();
private:
	int Init();
};

#endif