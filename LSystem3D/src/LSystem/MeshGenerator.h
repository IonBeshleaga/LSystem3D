#ifndef MESH_GENERATOR_CLASS_H
#define MESH_GENERATOR_CLASS_H


#include <stack>

#include "../Model/Mesh.h"
#include "LSystem.h"
#include "Configuration/MeshConfiguration.h"

struct section {
	std::vector<glm::vec3> positions;
};

struct stack_data {
	glm::vec3 pos;
	float angle;
	GLuint skeleton_indice;
	GLuint skin_indice;
};

class MeshGenerator {
public:
	MeshConfiguration mconfig;
	std::stack<stack_data> stk;

	glm::vec3 start_pos;
	float start_angle;


	std::vector<Vertex> skeleton_vertices;
	std::vector<GLuint> skeleton_indices;

	int section_size = 3;
	float radius = 0.5;

	std::vector<Vertex> skin_vertices;
	std::vector<GLuint> skin_indices;

	MeshGenerator();
	~MeshGenerator();

	void load_mesh_configuration(std::string mesh_config_path, std::string color_config_path);
	void GenerateMesh(std::string lsystem);
	Mesh getSkeletonMesh();
	Mesh getSkinMesh();
};

#endif
