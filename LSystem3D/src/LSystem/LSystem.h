#ifndef LSYSTEM_CLASS_H
#define LSYSTEM_CLASS_H

#include "../VBO/VBO.h"
#include <string>
#include <map>
#include <stack>

struct stack_data {
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

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	LSystem();
	~LSystem();

	void GenerateLSystem();
	void GenerateLSystemWord();
	void GenerateLSystemMesh();
	std::vector<Vertex> getVertices();
	std::vector<GLuint> getIndices();
}

#endif
