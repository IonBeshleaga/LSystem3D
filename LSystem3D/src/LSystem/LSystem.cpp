#include "LSystem.h"

#include <iostream>

LSystem::LSystem() {
	srand(time(NULL));
	axiom = "BBBBA";
	rules['A'] = "B[-A]+A";
	rules['B'] = "B";
	rules['['] = "[";
	rules[']'] = "]";
	rules['-'] = "-";
	rules['+'] = "+"; 

	lsystem_word = axiom;

	ls_iteration = 5;
	length = 0.5;
	angle = 35.f;

	start_angle = 90.f;
	start_pos = glm::vec3(0, -1, 0);
	rconfig = RulesConfiguration("res/test.wconfig", "asd", "asd");


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

		std::cout << "iteration " << k << ": " << lsystem_word << std::endl;
	}

	std::cout << "TEST RCONFIG A1 " << rconfig.getWRule('A') << std::endl;
	std::cout << "TEST RCONFIG A2 " << rconfig.getWRule('A') << std::endl;
	std::cout << "TEST RCONFIG A3 " << rconfig.getWRule('A') << std::endl;
	std::cout << "TEST RCONFIG A4 " << rconfig.getWRule('A') << std::endl;
	std::cout << "TEST RCONFIG A5 " << rconfig.getWRule('A') << std::endl;
	std::cout << "TEST RCONFIG A6 " << rconfig.getWRule('A') << std::endl;
	std::cout << "TEST RCONFIG A7 " << rconfig.getWRule('A') << std::endl;
	std::cout << "TEST RCONFIG A8 " << rconfig.getWRule('A') << std::endl;
	std::cout << "TEST RCONFIG A9 " << rconfig.getWRule('A') << std::endl;
	std::cout << "TEST RCONFIG A10 " << rconfig.getWRule('A') << std::endl;
	std::cout << "TEST RCONFIG B1 " << rconfig.getWRule('B') << std::endl;
	std::cout << "TEST RCONFIG B2 " << rconfig.getWRule('B') << std::endl;
	std::cout << "TEST RCONFIG B3 " << rconfig.getWRule('B') << std::endl;
	std::cout << "TEST RCONFIG B4 " << rconfig.getWRule('B') << std::endl;
	std::cout << "TEST RCONFIG 3 " << rconfig.getWRule('+') << std::endl;
	std::cout << "TEST RCONFIG 4 " << rconfig.getWRule('-') << std::endl;
	std::cout << "TEST RCONFIG 5 " << rconfig.getWRule('[') << std::endl;
	std::cout << "TEST RCONFIG 6 " << rconfig.getWRule(']') << std::endl;
}

void LSystem::GenerateLSystemMesh() {
	GLuint cur_ind = 0;
	GLuint last_ind = 0;
	float cur_angle = start_angle;
	glm::vec3 cur_pos = start_pos;
	float angle_rad;

	vertices.push_back({ cur_pos, glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec2(0,0) });
	

	for (int i = 1; i < lsystem_word.length(); i++) {
		angle_rad = glm::radians(cur_angle);
		if (lsystem_word[i] == 'A') {
			cur_pos += glm::vec3(length * std::cos(angle_rad), length * std::sin(angle_rad), 0);
			vertices.push_back({ cur_pos, glm::vec3(0,0,0), glm::vec3(0,1,0), glm::vec2(0,0) });
			
			indices.push_back(last_ind);
			indices.push_back(++cur_ind);
			last_ind = cur_ind;
		}
		else if (lsystem_word[i] == 'B') {
			cur_pos += glm::vec3(length * std::cos(angle_rad), length * std::sin(angle_rad), 0);
			vertices.push_back({ cur_pos, glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec2(0,0) });

			indices.push_back(last_ind);
			indices.push_back(++cur_ind);
			last_ind = cur_ind;
		}
		else if (lsystem_word[i] == '+') {
			cur_angle += angle;
		}
		else if (lsystem_word[i] == '-') {
			cur_angle -= angle;
		}
		else if (lsystem_word[i] == '[') {
			stk.push(stack_data{ cur_pos, cur_ind, cur_angle });
		}
		else if (lsystem_word[i] == ']') {
			cur_angle = stk.top().angle;
			last_ind = stk.top().indices;
			cur_pos = stk.top().pos;
			stk.pop();
		}
	}
	for (int i = 0; i < indices.size(); i++) {
		std::cout << indices[i] << ' ';
	}
}

std::vector<Vertex> LSystem::getVertices() {
	return vertices;
}
std::vector<GLuint> LSystem::getIndices() {
	return indices;
}