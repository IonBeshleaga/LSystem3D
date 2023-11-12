#include "RulesConfiguration.h"


RulesConfiguration::RulesConfiguration(std::string path) {
	
	load_config(path);
	
}

RulesConfiguration::RulesConfiguration() {
}


RulesConfiguration::~RulesConfiguration() {


}

std::string RulesConfiguration::getWRule(char symbol) {
	float r = (rand() % 10000)/100.f;

	

	std::vector<float> c=wrules[symbol].chances;

	
	int slot;
	float cur_chance = 0;
	for (slot = 0; slot < c.size(); slot++) {
		cur_chance += c[slot];
		if (r <= cur_chance) break;
	}



	return wrules[symbol].rules[slot] ;
}


void RulesConfiguration::load_config(std::string path) {
	std::ifstream in(path);
	
	int num_of_sym, num_of_rules;
	char symbol;
	std::vector<std::string> rules;
	std::vector<float> chances;
	
	
	in >> iteration;
	in >> axiom;
	in >> num_of_sym ; 
	
	for (int i = 0; i < num_of_sym; i++) { 
		in >> symbol;
		in >> num_of_rules;

		rules.clear(); chances.clear();
		rules.resize(num_of_rules); chances.resize(num_of_rules);
		float f;
		for (int k = 0; k < num_of_rules; k++) {
			in >> rules[k];
			in >> chances[k];
			
		}

		wrules.insert(std::make_pair(symbol, WRule{ rules, chances }));

	}


}
