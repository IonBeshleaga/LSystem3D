#include "../src/Model/Model.h"
#include "LSystem/LSystem.h"

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
	

	Shader shaderProgram("res/shader.vert", "res/shader.frag");
	Camera camera(WIN_WIDTH, WIN_HEIGHT, glm::vec3(0,0,5));

	std::vector<Vertex> line = {
		{glm::vec3(0.5,0.5, 0.0f), glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec2(0,0)},
		{glm::vec3(0.5, -0.5, 0.0f), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0)},
		{glm::vec3(-0.5, -0.5, 0.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec2(0, 0)},
		{glm::vec3(-0.5, 0.5, 0.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 1), glm::vec2(0, 0)}
	};

	std::vector<GLuint> indices = {
		0,1,2,3
	};

	std::vector<Texture> textures ;


	Mesh mesh(line, indices, textures);
	glm::mat4 model = glm::mat4(1.f);
	
	shaderProgram.Activate();
	
	LSystem ls;
	ls.GenerateLSystem();

	//main loop
	glClearColor(0, 0, 0, 1);
	while (!glfwWindowShouldClose(window)) {
			
		camera.Input(window);
		camera.Update(45, 0.1, 100);
		//camera.Matrix(shaderProgram, "camMatrix");

		//model = glm::rotate(model, glm::radians(5.f) * (float)glfwGetTime(), glm::vec3(1, 0, 0));
		shaderProgram.setMat4f("modelMatrix", 1, GL_FALSE, glm::value_ptr(model));

		glClear(GL_COLOR_BUFFER_BIT);


		mesh.Draw(shaderProgram, camera);

		glfwSwapBuffers(window);
				
		glfwPollEvents();
	}

	shaderProgram.Delete();
	

	//deinit window and glfw
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}