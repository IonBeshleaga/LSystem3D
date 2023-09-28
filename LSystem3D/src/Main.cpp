#include "../src/Model/Model.h"
#include "LSystem/MeshGenerator.h"

int main() {
	srand(time(NULL));
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


	LSystem ls;
	MeshGenerator mg;

	ls.load_lsystem_configuration("res/test.wconfig");
	ls.GenerateLSystem();
	
	mg.load_mesh_configuration("res/test.mconfig", "res/test.cconfig");
	mg.GenerateMesh(ls.getLSystem());
	

	Mesh mesh = mg.getMesh();
	glm::mat4 model = glm::mat4(1.f);
	
	shaderProgram.Activate();
	

	std::cout << "Lsystem word " << ls.getLSystem() << std::endl;
	/*std::cout << "Vertices of mesh " << std::endl;
	for (int i = 0; i < mesh.vertices.size(); i++) {
		std::cout << mesh.vertices[i].position.x << ' ' << mesh.vertices[i].position.y << ' ' << mesh.vertices[i].position.z << std::endl;
		std::cout << mesh.vertices[i].color.x << ' ' << mesh.vertices[i].color.y << ' ' << mesh.vertices[i].color.z << std::endl << std::endl;
	}
	std::cout << "Indices of mesh" << std::endl;
	for (int i = 0; i < mesh.indices.size(); i++) {
		std::cout << mesh.indices[i] << ' ' << std::endl;
	}*/
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