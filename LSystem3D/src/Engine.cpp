#include "Engine.h"

Engine::Engine() {
	srand(time(NULL));
	int window_width = 1200;
	int window_height = 900;
	const char* title = "LSystem";
	glm::vec3 camera_pos = glm::vec3(0, 0, 5);
	window = new Window(window_width, window_height, title);
	
	camera = new Camera(window_width, window_height, camera_pos);
	shader = new Shader("res/shader.vert", "res/shader.frag");

	lsystem = new LSystem("res/test.wconfig");
	meshGen = new MeshGenerator("res/test.mconfig", "res/test.cconfig");

}

Engine::~Engine() {
	shader->Delete();
	delete shader;
	delete camera;
	delete window;
	delete lsystem;
	delete meshGen;

	/*ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();*/
}


void Engine::ProccesInput(float dt) {
	camera->Input(window->window,dt);
}

void Engine::Update(float dt) {
	glm::vec2 win_size = window->getSize();
	camera->Update(45, 0.1, 100, win_size.x, win_size.y);
}

void Engine::Draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	*/
	
	for (auto it : models) {
		shader->setMat4f("modelMatrix", 1, GL_FALSE, glm::value_ptr(it.second.modelMatrix));
		it.second.mesh.Draw(*shader, *camera, it.second.drawingType);
	}
	/*	
	ImGui::Begin("Window");
	ImGui::Text("Hello Text");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	*/
	
	window->swapBuffers();


}

void Engine::Run() {
	//dt
	float last_time = glfwGetTime();
	float current_time;
	float deltaTime;

	//Gen Models
	lsystem->GenerateLSystem();
	meshGen->GenerateMesh(lsystem->getLSystem());

	glm::mat4 modelMatrix = glm::mat4(1.f);
	models["skeleton"] = model_object{ meshGen->getSkeletonMesh(), modelMatrix, GL_LINES };
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(6, 0, 0));
	models["skin"] = model_object{ meshGen->getSkinMesh(), modelMatrix, GL_TRIANGLES };

	//ImGui
	/*IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window->window, true);
	ImGui_ImplOpenGL3_Init("#version 330");*/
	//opengl sets
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.26, 0.26, 0.28, 1);
	
	//loop
	while (!window->isShouldClose()) {
		current_time = glfwGetTime();
		deltaTime = current_time - last_time;
		last_time = current_time;
		//if (!io.WantCaptureMouse) {
		ProccesInput(deltaTime);
		//	io.MouseDrawCursor = false;
		//}
		Update(deltaTime);
		Draw();
		window->pollEvents();
	}
}