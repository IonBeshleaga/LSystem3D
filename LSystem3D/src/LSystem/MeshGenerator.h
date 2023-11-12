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
	glm::vec3 radius;
	glm::vec3 dir;
	int skel_indice;
	int skin_indice;
	int level;
};

class MeshGenerator {
public:
	MeshConfiguration mconfig;
	std::stack<stack_data> stk;

	glm::vec3 start_pos;
	float start_angle;


	std::vector<Vertex> skeleton_vertices;
	std::vector<GLuint> skeleton_indices;

	int section_size = 4;
	float radius = 1;
	float radius_change = 0.9;

	std::vector<Vertex> skin_vertices;
	std::vector<GLuint> skin_indices;

public:
	MeshGenerator();
	MeshGenerator(std::string mesh_config_path, std::string color_config_path);
	~MeshGenerator();

	void load_mesh_configuration(std::string mesh_config_path, std::string color_config_path);
	void setMeshConfiguration(MeshConfiguration meshConfiguration);
	void GenerateMesh(std::string lsystem);
	MeshConfiguration getMeshConfiguration();
	Mesh getSkeletonMesh();
	Mesh getSkinMesh();
};

#endif
