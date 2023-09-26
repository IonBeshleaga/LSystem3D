#ifndef LSYSTEM_CLASS_H
#define LSYSTEM_CLASS_H

#include "../VBO/VBO.h"
#include "Configuration/RulesConfiguration.h"
#include "Configuration/MeshConfiguration.h"
#include <string>
#include <map>
#include <stack>
#include <math.h>


struct stack_data {
	glm::vec3 pos;
	GLuint indices;
	float angle;
};

class LSystem {
public:
	std::string lsystem_word;
	std::string axiom;
	std::map<char, std::string> rules;
	std::stack<stack_data> stk;
	int ls_iteration;

	float length;
	float angle;
	float start_angle;
	glm::vec3 start_pos;
	
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	RulesConfiguration rconfig;

	LSystem();
	~LSystem();

	void GenerateLSystem();
	void GenerateLSystemWord();
	void GenerateLSystemMesh();
	std::vector<Vertex> getVertices();
	std::vector<GLuint> getIndices();
};

#endif
