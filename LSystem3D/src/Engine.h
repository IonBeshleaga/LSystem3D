#ifndef ENGINE_CLASS_H
#define ENGINE_CLASS_H

#include "Window/Window.h"
#include "LSystem/MeshGenerator.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include <filebrowser/imfilebrowser.h>

struct model_object{
	Mesh mesh;
	glm::mat4 modelMatrix;
	GLenum drawingType;
};

class Engine {
public:
	Window* window;

	Camera* camera;
	Shader* shader;

	LSystem* lsystem;
	MeshGenerator* meshGen;
	
	MeshConfiguration CurMeshConfiguration;
	RulesConfiguration CurRulesConfiguration;

	glm::vec3 skyColor;
	std::string modelType;
	std::map<std::string, model_object> models;

	ImGui::FileBrowser saveFileDialog;
	ImGui::FileBrowser loadFileDialog;
public:
	Engine();
	~Engine();


	void ProccesInput(float dt);
	void Update(float dt);
	void Draw();

	void Run();
private:
	void generateModels();
	void setMeshConfig();
	void setRulesConfig();
	void loadConfiguration(std::string path);
	void saveConfiguration(std::string path);
	void DrawImGui();
};

#endif