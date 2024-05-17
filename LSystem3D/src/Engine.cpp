#include "Engine.h"

namespace ImGui
{
	// ImGui::InputText() with std::string
	// Because text input needs dynamic resizing, we need to setup a callback to grow the capacity
	IMGUI_API bool  InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
	IMGUI_API bool  InputTextMultiline(const char* label, std::string* str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
	IMGUI_API bool  InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
}


//SET UP  
Engine::Engine() {
	srand(time(NULL));
	int window_width = 1200;
	int window_height = 900;
	const char* title = "LSystem";
	glm::vec3 camera_pos = glm::vec3(0, 0, 5);
	window = new Window(window_width, window_height, title);
	
	camera = new Camera(window_width, window_height, camera_pos);
	shader = new Shader("res/shader.vert", "res/shader.frag");

	loadConfiguration("res/test.lsconfig");
	modelType = "skeleton";
	skyColor = glm::vec3(0.26, 0.26, 0.28);
	/*
	lsystem = new LSystem("res/test.wconfig");
	meshGen = new MeshGenerator("res/test.mconfig", "res/test.cconfig");
	*/


}

Engine::~Engine() {
	shader->Delete();
	delete shader;
	delete camera;
	delete window;
	delete lsystem;
	delete meshGen;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


void Engine::ProccesInput(float dt) {
	camera->Input(window->window,dt);
}

void Engine::Update(float dt) {
	//update window and camera sizes
	glm::vec2 win_size = window->getSize();
	camera->Update(45, 0.1, 100, win_size.x, win_size.y);
}

void Engine::Draw() {
	glClearColor(skyColor.x, skyColor.y, skyColor.z, 1);
	//std::cout << skyColor.x << ' ' << skyColor.y << ' ' << skyColor.z << '\n';
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
	for (auto it : models) {
		shader->setMat4f("modelMatrix", 1, GL_FALSE, glm::value_ptr(it.second.modelMatrix));
		it.second.mesh.Draw(*shader, *camera, it.second.drawingType);
	}*/
		
	shader->setMat4f("modelMatrix", 1, GL_FALSE, glm::value_ptr(models[modelType].modelMatrix));
	models[modelType].mesh.Draw(*shader, *camera, models[modelType].drawingType);

	DrawImGui();
		
	window->swapBuffers();
}

void Engine::Run() {
	//dt
	float last_time = glfwGetTime();
	float current_time;
	float deltaTime;

	//Gen Models
	generateModels();
	//Init ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window->window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	//opengl sets
	glEnable(GL_DEPTH_TEST);
	
	
	//main loop
	while (!window->isShouldClose()) {

		//update dt
		current_time = glfwGetTime();
		deltaTime = current_time - last_time;
		last_time = current_time;

		//imgui update
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//If ImGui is not processed than process input for camera
		if (!io.WantCaptureMouse) {
			ProccesInput(deltaTime);
		}
		Update(deltaTime);
		Draw();
		window->pollEvents();
	}
}



void Engine::generateModels() {
	//set configurations
	setMeshConfig();
	setRulesConfig();
	//generate lsystem
	lsystem->GenerateLSystem();
	//generate mesh based on lsystem
	meshGen->GenerateMesh(lsystem->getLSystem());
	std::cout << "Engine:	Lsystem: " << lsystem->getLSystem() << std::endl;
	glm::mat4 modelMatrix = glm::mat4(1.f);
	models["skeleton"] = model_object{ meshGen->getSkeletonMesh(), modelMatrix, GL_LINES };
	models["skin"] = model_object{ meshGen->getSkinMesh(), modelMatrix, GL_TRIANGLES};
	models["line_strip"] = model_object{ meshGen->getSkinMesh(), modelMatrix, GL_LINE_STRIP};


}


void Engine::setMeshConfig() {
	lsystem->setRulesConfiguration(CurRulesConfiguration);
}
void Engine::setRulesConfig() {
	meshGen->setMeshConfiguration(CurMeshConfiguration);
}


void Engine::loadConfiguration(std::string path) {
	if (lsystem != NULL) delete lsystem;
	if (meshGen != NULL) delete meshGen;
	
	std::ifstream in(path);
	std::string line;
	std::vector<std::string> data; 
	data.resize(3);
	int cur_file = 0;
	while (in >> line) {
		if (line == "#WORD") {
			cur_file = 0;
		}else if (line == "#MESH") {
			cur_file = 1;
		}else if(line == "#COLOR") {
			cur_file = 2;
		}else {
			data[cur_file] += line + " ";
		}
	}
	std::cout << "Hello before lsystem" << std::endl;
	lsystem = new LSystem(data[0]);

	std::cout << "Hello before meshconfig" << std::endl;
	meshGen = new MeshGenerator(data[1], data[2]);

	std::cout << "Hello before lsystem getting" << std::endl;
	CurRulesConfiguration = lsystem->getRulesConfiguration();
	std::cout << "Hello before meshconfig getting" << std::endl;
	CurMeshConfiguration = meshGen->getMeshConfiguration();
}



void Engine::DrawImGui() {
	
	static bool alphabetWindow = false;
	static bool meshWindow = false;
	static bool settingsWindow = false;
	static bool newRuleWindow = false;
	static bool newColorWindow = false;
	static bool newSymbolWindow = false;
	if(ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			ImGui::MenuItem("Open", NULL, false, true);
			ImGui::MenuItem("Save", NULL, false, true);
			ImGui::EndMenu();
		}
		if (ImGui::Button("Generate")) {
			generateModels();
		}
		if (ImGui::Button("Alphabet")) alphabetWindow = !alphabetWindow;
		if (ImGui::Button("Mesh")) meshWindow = !meshWindow;
		if(ImGui::Button("Settings")) settingsWindow = !settingsWindow;
		ImGui::EndMainMenuBar();
	}
/*	ImGui::Begin("Window");

	
	*/

	//------------------ ALPHABET -------------------------

	if (alphabetWindow) {
		ImGui::Begin("Alphabet", &alphabetWindow, 0);
		ImGui::InputInt("Iteration", &CurRulesConfiguration.iteration);
		ImGui::InputText("Axiom", &CurRulesConfiguration.axiom);
		
		ImGui::Spacing();
		
		bool deleteSymbol = false;
		
		//GET ALL SYMBOLS
		static std::string selectSymbols;
		selectSymbols.resize(2 * CurRulesConfiguration.wrules.size());
		static int curSymbolItem = 0;
		static char curSymbol;
		int i = 0;
		for (auto it = CurRulesConfiguration.wrules.begin(); it != CurRulesConfiguration.wrules.end(); it++) {
			selectSymbols[i * 2] = it->first;
			selectSymbols[(i * 2) + 1] = '\0';
			i++;
		}

		// COMBO WITH SYMBOLS
		ImGui::Combo("Symbol", &curSymbolItem, selectSymbols.c_str(), CurRulesConfiguration.wrules.size());


		// RULES FO SYMBOLS 
		curSymbol = selectSymbols[curSymbolItem * 2];
		// DELETE SYMBOL
		if (ImGui::Button("Delete Symbol")) {
			CurRulesConfiguration.deleteWRule(curSymbol);
			CurMeshConfiguration.deleteMRule(curSymbol);
			CurMeshConfiguration.deleteCRule(curSymbol);
			auto it = CurRulesConfiguration.wrules.begin();
			curSymbol = it->first;
		}

		ImGui::Spacing();
		// RULES TYPE AND DATA
		switch (CurMeshConfiguration.mrules[curSymbol].type)
		{
		case stack:
			ImGui::Text("Type: Stack");
			static int radio_stack_type;
			radio_stack_type = CurMeshConfiguration.mrules[curSymbol].data[0];

			ImGui::RadioButton("Save##CurentSymbol", &radio_stack_type, save);
			ImGui::RadioButton("Load##CurentSymbol", &radio_stack_type, load);

			CurMeshConfiguration.mrules[curSymbol].data[0] = radio_stack_type;

			break;
		case branch:
			ImGui::Text("Type: Branch");
			ImGui::InputFloat("Length##CurentSymbol", &CurMeshConfiguration.mrules[curSymbol].data[0]);
			break;
		case leaf:
			ImGui::Text("Type: Leaf");
			ImGui::InputFloat("Length##CurentSymbol", &CurMeshConfiguration.mrules[curSymbol].data[0]);
			break;
		case rotate:
			ImGui::Text("Type: Rotation");
			ImGui::DragFloatRange2("X range##CurentSymbol", &CurMeshConfiguration.mrules[curSymbol].data[0], &CurMeshConfiguration.mrules[curSymbol].data[1], 0.25f, -360.f, 360.f, "Min: %.1f", "Max: %.1f ", ImGuiSliderFlags_AlwaysClamp);
			ImGui::DragFloatRange2("Y range##CurentSymbol", &CurMeshConfiguration.mrules[curSymbol].data[2], &CurMeshConfiguration.mrules[curSymbol].data[3], 0.25f, -360.f, 360.f, "Min: %.1f", "Max: %.1f ", ImGuiSliderFlags_AlwaysClamp);
			ImGui::DragFloatRange2("Z range##CurentSymbol", &CurMeshConfiguration.mrules[curSymbol].data[4], &CurMeshConfiguration.mrules[curSymbol].data[5], 0.25f, -360.f, 360.f, "Min: %.1f", "Max: %.1f ", ImGuiSliderFlags_AlwaysClamp);
			break;
		default:
			break;
		}
		ImGui::Spacing();
		if (ImGui::TreeNode("Rules")) {
			// ADD RULES FOR SYMBOL
			if (ImGui::Button("Add New Rule")) newRuleWindow = true;
			if (newRuleWindow) {
				ImGui::Begin("New Rule", &newRuleWindow, 0);
				static std::string add_rule;
				static float add_chance;
				ImGui::InputText("Input new rule", &add_rule);
				ImGui::InputFloat("Input new chance", &add_chance);
				if (ImGui::Button("Add rule")) {
					CurRulesConfiguration.wrules[curSymbol].chances.push_back(add_chance);
					CurRulesConfiguration.wrules[curSymbol].rules.push_back(add_rule);
				}
				ImGui::End();
			}
			//EXISTINGS RULES
			ImGui::Spacing();
			for (int i = 0; i < CurRulesConfiguration.wrules[curSymbol].rules.size(); i++) {
				std::string labelr = "Rule " + std::to_string(i + 1);
				std::string labelc = "Chance " + std::to_string(i + 1);
				std::string button_text = "Delete Rule " + std::to_string(i + 1);

				ImGui::InputText(labelr.c_str(), &CurRulesConfiguration.wrules[curSymbol].rules[i]);
				ImGui::InputFloat(labelc.c_str(), &CurRulesConfiguration.wrules[curSymbol].chances[i]);
				if (ImGui::Button(button_text.c_str())) {

					std::cout << "Deleted" << std::endl;
					CurRulesConfiguration.wrules[curSymbol].chances.erase(CurRulesConfiguration.wrules[curSymbol].chances.begin() + i);
					CurRulesConfiguration.wrules[curSymbol].rules.erase(CurRulesConfiguration.wrules[curSymbol].rules.begin() + i);

				}
				//it.second.rules[i] = std::string(inputText);
			}
			ImGui::TreePop();
		}
		
		// ----------------------------------------- COLOR RULES --------------------------- 
		ImGui::Spacing();
		if (CurMeshConfiguration.mrules[curSymbol].type == branch || CurMeshConfiguration.mrules[curSymbol].type == leaf) {
			
			if (ImGui::TreeNode("Color")) {
				// ADD COLOR
				if (ImGui::Button("Add New Color")) newColorWindow = true;
				if (newColorWindow) {
					ImGui::Begin("New Color", &newColorWindow, 0);
					static glm::vec3 add_color;
					static float add_chance_for_color;
					ImGui::ColorEdit3("Input new color", (float*)&add_color);
					ImGui::InputFloat("Input new chance##color", &add_chance_for_color);
					if (ImGui::Button("Add color")) {
						CurMeshConfiguration.crules[curSymbol].chances.push_back(add_chance_for_color);
						CurMeshConfiguration.crules[curSymbol].color.push_back(add_color);
					}
					ImGui::End();
				}
				//EXISTINGS COLOR
				ImGui::Spacing();
				for (int i = 0; i < CurMeshConfiguration.crules[curSymbol].color.size(); i++) {
					std::string label_color = "Color " + std::to_string(i + 1) + "##color";
					std::string label_chance = "Chance " + std::to_string(i + 1) + "##color";
					std::string button_text = "Delete color " + std::to_string(i + 1) + "##color";
					ImGui::ColorEdit3(label_color.c_str(), (float*)&CurMeshConfiguration.crules[curSymbol].color[i]);
					ImGui::InputFloat(label_chance.c_str(), &CurMeshConfiguration.crules[curSymbol].chances[i]);
					if (ImGui::Button(button_text.c_str())) {

						std::cout << "Deleted" << std::endl;
						CurMeshConfiguration.crules[curSymbol].chances.erase(CurMeshConfiguration.crules[curSymbol].chances.begin() + i);
						CurMeshConfiguration.crules[curSymbol].color.erase(CurMeshConfiguration.crules[curSymbol].color.begin() + i);

					}
				}
			
			
			ImGui::TreePop();
			}
		}

		//-------------------------------------- NEW SYMBOL ------------------------------------------------------
		if (ImGui::Button("Add New Symbol")) newSymbolWindow = true;
		if (newSymbolWindow) {
			ImGui::Begin("New Symbol", &newSymbolWindow, 0);
			static std::string inputNewCaracter;
			static MRuleType inputNewType;
			static std::vector<float> inputNewData(1, 0);
			static int curItem = 0;
			std::vector<const char*> select = { "stack", "leaf", "branch", "rotate" };
			ImGui::Spacing();
			ImGui::InputText("New Symbol", &inputNewCaracter);
			if (inputNewCaracter.length() > 1) inputNewCaracter.resize(1);
			if (ImGui::Combo("Type", &curItem, select.data(), 4)) {
				switch (curItem) {
				case 0:
					inputNewType = stack;
					inputNewData.resize(1);
					inputNewData[0] = 0;
					break;
				case 1:
					inputNewType = leaf;
					inputNewData.resize(1);
					inputNewData[0] = 1;
					break;
				case 2:
					inputNewType = branch;
					inputNewData.resize(1);
					inputNewData[0] = 1;
					break;
				case 3:
					inputNewType = rotate;
					inputNewData.resize(6);
					inputNewData[0] = 45; inputNewData[1] = 45;
					inputNewData[2] = 0; inputNewData[3] = 0;
					inputNewData[4] = 0; inputNewData[5] = 0;
					break;
				}
			}

			//TYPE FOR NEW SYMBOL
			switch (inputNewType)
			{
			case stack:
				static int radio_new_stack_type = inputNewData[0];

				ImGui::RadioButton("Save##NewSymbol", &radio_new_stack_type, save);
				ImGui::RadioButton("Load##NewSymbol", &radio_new_stack_type, load);

				inputNewData[0] = radio_new_stack_type;

				break;
			case branch:
				ImGui::InputFloat("Length##NewSymbol", &inputNewData[0]);
				break;
			case leaf:
				ImGui::InputFloat("Length##NewSymbol", &inputNewData[0]);
				break;
			case rotate:
				ImGui::DragFloatRange2("X range##NewSymbol", &inputNewData[0], &inputNewData[1], 0.25f, -360.f, 360.f, "Min: %.1f", "Max: %.1f ", ImGuiSliderFlags_AlwaysClamp);
				ImGui::DragFloatRange2("Y range##NewSymbol", &inputNewData[2], &inputNewData[3], 0.25f, -360.f, 360.f, "Min: %.1f", "Max: %.1f ", ImGuiSliderFlags_AlwaysClamp);
				ImGui::DragFloatRange2("Z range##NewSymbol", &inputNewData[4], &inputNewData[5], 0.25f, -360.f, 360.f, "Min: %.1f", "Max: %.1f ", ImGuiSliderFlags_AlwaysClamp);
				break;
			default:
				break;
			}

			if (ImGui::Button("Add Symbol")) {
				std::vector<std::string> rules(1); rules[0] = inputNewCaracter[0];

				std::vector < glm::vec3> colors = { glm::vec3(1,1,1) };
				std::vector<float> chances = { 100 };
				curSymbol = inputNewCaracter[0];
				CurRulesConfiguration.wrules.insert(std::make_pair(inputNewCaracter[0], WRule{ rules, chances }));
				CurMeshConfiguration.mrules.insert(std::make_pair(inputNewCaracter[0], MRule{ inputNewType, inputNewData }));
				CurMeshConfiguration.crules.insert(std::make_pair(inputNewCaracter[0], CRule{ colors, chances }));
			}
			ImGui::End();
		}


		ImGui::End();
	}
	
	// ----------------------------------- MESH -----------------------------------------
	
	if (meshWindow) {
		static int curSymbolItem;
		const char* list[] = { "skeleton", "skin", "line_strip" };
		float start_pos[] = { meshGen->start_pos.x, meshGen->start_pos.y, meshGen->start_pos.z };
		
		ImGui::Begin("Mesh setting", &meshWindow, 0);
		ImGui::InputFloat3("Start Position", start_pos);
		meshGen->start_pos = glm::vec3(start_pos[0], start_pos[1], start_pos[2]);
		ImGui::SliderFloat("Start Angle", &meshGen->start_angle, -360, 360);
		ImGui::DragInt("Section Size", &meshGen->section_size, 2,32);
		ImGui::DragFloat("Radius", &meshGen->radius, 0.1,0.001, 100);
		ImGui::DragFloat("Radius Change", &meshGen->radius_change, 0.1,0.001, 100);
		ImGui::DragFloat("Scale", &meshGen->scale, 0.1, 0.001, 100);
		ImGui::Combo("Mesh Type", &curSymbolItem,list , 3);
		modelType = list[curSymbolItem];
		
		ImGui::End();
	}

	// ----------------------------------- SETTINGS -----------------------------------------
	if (settingsWindow) {
		ImGui::Begin("Settings", &settingsWindow, 0);
		ImGui::Text("Control \n W - Forward \n S - Back \n A - Left \n D - Right \n Q - Down \n E - Up\n Shift - Speed up \n Mouse for camera control \n");
		ImGui::ColorEdit3("SkyColor", (float*)&skyColor);
		ImGui::End();
	}
	/*
	ImGui::Begin("Window");
	// ---------------------------------- SAVE CONFIGURATION ------------------------------------
	static std::string file_save_name;
	ImGui::InputText("Input save file name", &file_save_name);
	if (ImGui::Button("Save Configuration")) {
		std::ofstream out("res/"+file_save_name + ".lsconfig");
		out << "#WORD" << std::endl;
		out << CurRulesConfiguration.getConfiguration();
		out << "#MESH" << std::endl;
		out << CurMeshConfiguration.getMeshConfiguration();
		out << "#COLOR" << std::endl;
		out << CurMeshConfiguration.getColorConfiguration();
		out.close();
	}

	// ---------------------------------- LOAD CONFIGURATION --------------------------------------
	static std::string file_load_name;
	ImGui::InputText("Input load file name", &file_load_name);
	if (ImGui::Button("Load Configuration")) {
		loadConfiguration(file_load_name);
	}


	ImGui::End();*/

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}