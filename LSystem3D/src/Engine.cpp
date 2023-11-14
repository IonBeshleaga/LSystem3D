#include "Engine.h"

namespace ImGui
{
	// ImGui::InputText() with std::string
	// Because text input needs dynamic resizing, we need to setup a callback to grow the capacity
	IMGUI_API bool  InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
	IMGUI_API bool  InputTextMultiline(const char* label, std::string* str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
	IMGUI_API bool  InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
}

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


	CurRulesConfiguration = lsystem->getRulesConfiguration();
	CurMeshConfiguration = meshGen->getMeshConfiguration();

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
	
	for (auto it : models) {
		shader->setMat4f("modelMatrix", 1, GL_FALSE, glm::value_ptr(it.second.modelMatrix));
		it.second.mesh.Draw(*shader, *camera, it.second.drawingType);
	}
		
	ImGui::Begin("Window");
	if (ImGui::CollapsingHeader("LSystem setting")) {
		ImGui::InputInt("Iteration", &lsystem->ls_iteration);
		ImGui::InputText("Axiom", &lsystem->axiom);
		bool deleteSymbol = false;
		if (ImGui::TreeNode("Rules")) {
			char c[3];  c[1] = '\n'; c[2] = '\0';
			for (auto it = CurRulesConfiguration.wrules.begin(); it != CurRulesConfiguration.wrules.end();) {
				const char s = it->first;
				c[0] = s;
				if (ImGui::TreeNode(c)) {

					for (int i = 0; i < it->second.rules.size(); i++) {
						

						std::string labelr = "Rule " + std::to_string(i + 1);
						std::string labelc = "Chance " + std::to_string(i + 1);
						std::string button_text = "Delete Rule " + std::to_string(i + 1);
						ImGui::InputText(labelr.c_str(), &CurRulesConfiguration.wrules[s].rules[i]);
						ImGui::InputFloat(labelc.c_str(), &CurRulesConfiguration.wrules[s].chances[i]);
						if (ImGui::Button(button_text.c_str())) {

							std::cout << "Deleted" << std::endl;
							CurRulesConfiguration.wrules[s].chances.erase(CurRulesConfiguration.wrules[s].chances.begin()+i);
							CurRulesConfiguration.wrules[s].rules.erase(CurRulesConfiguration.wrules[s].rules.begin() + i);
						
						}
						//it.second.rules[i] = std::string(inputText);
					}

					static std::string add_rule;
					static float add_chance;
					ImGui::InputText("Input new rule", &add_rule);
					ImGui::InputFloat("Input new chance", &add_chance);
					if (ImGui::Button("Add rule")) {
						CurRulesConfiguration.wrules[s].chances.push_back(add_chance);
						CurRulesConfiguration.wrules[s].rules.push_back(add_rule);
					}
					if (ImGui::Button("Delete this Symbol")) {
						deleteSymbol = true;;
					}
					ImGui::TreePop();
					
				}
				if (deleteSymbol) {
					CurRulesConfiguration.wrules.erase(it++);
					deleteSymbol = false;
				}
				else {
					it++;
				}
			}
			
			static std::string inputNewCaracter;
			ImGui::InputText("New Symbol", &inputNewCaracter);
			if (inputNewCaracter.length() > 1) inputNewCaracter.resize(1);

			if (ImGui::Button("Add new Symbol")) {
				std::vector<std::string> rules(1); rules[0] = inputNewCaracter[0];
				std::vector<float> chances = { 100 };
				CurRulesConfiguration.wrules.insert(std::make_pair(inputNewCaracter[0], WRule{ rules, chances }));
			}


			ImGui::TreePop();
			
		}

	}
	if (ImGui::CollapsingHeader("Mesh setting")) {
		ImGui::InputInt("Section size", &meshGen->section_size);
		ImGui::InputFloat("Radius", &meshGen->radius);
		ImGui::InputFloat("Radius Change", &meshGen->radius_change);
	}
	if (ImGui::Button("Generate")) {
		generateModels();
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
	window->swapBuffers();


}

void Engine::Run() {
	//dt
	float last_time = glfwGetTime();
	float current_time;
	float deltaTime;

	//Gen Models
	generateModels();
	//ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window->window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	//opengl sets
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.26, 0.26, 0.28, 1);
	
	//loop
	while (!window->isShouldClose()) {
		current_time = glfwGetTime();
		deltaTime = current_time - last_time;
		last_time = current_time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (!io.WantCaptureMouse) {
			ProccesInput(deltaTime);
		}
		Update(deltaTime);
		Draw();
		window->pollEvents();
	}
}



void Engine::generateModels() {
	setMeshConfig();
	setRulesConfig();
	lsystem->GenerateLSystem();
	meshGen->GenerateMesh(lsystem->getLSystem());
	std::cout << "Engine:	Lsystem: " << lsystem->getLSystem() << std::endl;
	glm::mat4 modelMatrix = glm::mat4(1.f);
	models["skeleton"] = model_object{ meshGen->getSkeletonMesh(), modelMatrix, GL_LINES };
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 0, 0));
	models["skin"] = model_object{ meshGen->getSkinMesh(), modelMatrix, GL_TRIANGLES };

}


void Engine::setMeshConfig() {
	lsystem->setRulesConfiguration(CurRulesConfiguration);
}
void Engine::setRulesConfig() {
	meshGen->setMeshConfiguration(CurMeshConfiguration);
}