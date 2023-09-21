#include "LSystem.h"

#include <iostream>

LSystem::LSystem() {
	axiom = "A";
	rules['A'] = "B[-A]+A";
	rules['B'] = "BB";
	rules['['] = "[";
	rules[']'] = "]";
	rules['-'] = "-";
	rules['+'] = "+";

	lsystem_word = axiom;

	ls_iteration = 5;
	length = 0.05;
	angle = 45.f;

}

LSystem::~LSystem() {

}

void LSystem::GenerateLSystem() {
	GenerateLSystemWord();
	GenerateLSystemMesh();
}
void LSystem::GenerateLSystemWord() {
	for (int k = 0; k < ls_iteration; k++) {
		std::string tmp = "";
		for (int i = 0; i < lsystem_word.length(); i++) {
			tmp += rules[lsystem_word[i]];
		}
		lsystem_word = tmp;
	}
	std::cout << lsystem_word << std::endl;
}

void LSystem::GenerateLSystemMesh() {

}

std::vector<Vertex> LSystem::getVertices() {
	return vertices;
}
std::vector<GLuint> LSystem::getIndices() {
	return indices;
}