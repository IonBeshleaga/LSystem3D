#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main() {

	//window data
	const int WIN_WIDTH = 800;
	const int WIN_HEIGHT = 600;
	const char* WIN_TITLE = "3D_LSYSTEM";

	//window object
	GLFWwindow* window;

	//window hint
	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Init glfw
	if (!glfwInit()) {
		std::cerr << "GLFW INITIALIZATION WAS FAILED" << std::endl;
		return -1;
	}

	//Create window and error check
	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL);
	if (window == NULL) {
		std::cerr << "GLFW WINDOW CREATING WAS FAILED" << std::endl;
		glfwTerminate();
		return -1;
	}
	//make opengl context
	glfwMakeContextCurrent(window);

	//Init glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW INITIALIZATION WAS FAILED" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}
	

	//main loop
	glClearColor(1, 0, 0, 1);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	//deinit window and glfw
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}