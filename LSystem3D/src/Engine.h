#ifndef ENGINE_CLASS_H
#define ENGINE_CLASS_H

#include "Window/Window.h"
#include "LSystem/MeshGenerator.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

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
	
	std::map<std::string, model_object> models;
public:
	Engine();
	~Engine();


	void ProccesInput(float dt);
	void Update(float dt);
	void Draw();

	void Run();
private:
	void generateModels();
};

#endif