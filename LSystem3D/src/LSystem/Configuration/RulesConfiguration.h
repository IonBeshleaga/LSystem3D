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


class RulesConfiguration {
public:
	std::map <char, WRule> wrules;
	
	int iteration;
	std::string axiom;
	RulesConfiguration();
	RulesConfiguration(std::string path);
	~RulesConfiguration();

	std::string getWRule(char symbol);

private:
	void load_config(std::string path);
};

#endif

