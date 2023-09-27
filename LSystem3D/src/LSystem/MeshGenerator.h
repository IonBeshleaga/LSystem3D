#ifndef MESH_GENERATOR_CLASS_H
#define MESH_GENERATOR_CLASS_H


#include <stack>

#include "../Model/Mesh.h"
#include "LSystem.h"
#include "Configuration/MeshConfiguration.h"


struct stack_data {
	glm::vec3 pos;
	float angle;
	GLuint indices;
};

class MeshGenerator {
public:
	MeshConfiguration mconfig;
	std::stack<stack_data> stk;

	glm::vec3 start_pos;
	float start_angle;


	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	MeshGenerator();
	~MeshGenerator();

	void load_mesh_configuration(std::string mesh_config_path, std::string color_config_path);
	void GenerateMesh(std::string lsystem);
	Mesh getMesh();

};

#endif
