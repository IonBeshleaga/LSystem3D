#ifndef RULES_CONFIG_CLASS_H
#define RULES_CONFIG_CLASS_H

#include <glm/glm.hpp>
#include <string>
#include <map>
#include <stack>
#include <fstream>
#include <vector>
#include <random>
#include <iostream>

struct WRule {
	std::vector<std::string> rules;
	std::vector<float> chances;
};

struct MRule {};
struct CRule {};
struct mrule_data {

};

class RulesConfiguration {
public:
	std::map <char, WRule> wrules;
	std::map <char, MRule> mrules;
	std::map <char, CRule> crules;
	
	int iteration;
	std::string axiom;
	glm::vec3 start_pos;
	float start_angle;
	RulesConfiguration();
	RulesConfiguration(std::string lsystem_path, std::string mesh_path, std::string color_path);
	~RulesConfiguration();

	std::string getWRule(char symbol);
	/*mrule_data getMRule(char symbol);
	glm::vec3 getCRule(char symbol);*/



private:
	void load_lsystem_config(std::string path);
	void load_mesh_config(std::string path);
	void load_color_config(std::string path);



};

#endif

