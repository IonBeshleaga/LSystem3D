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
	for (slot = 0; slot < c.size()-2; slot++) {
		if ((c[slot] <= r) && (r < c[slot + 1])) break;
	}



	return wrules[symbol].rules[slot] ;
}


void RulesConfiguration::load_config(std::string path) {
	std::ifstream in(path);
	
	int num_of_sym, num_of_rules;
	float chance;
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
		rules.resize(num_of_rules); chances.resize(num_of_rules+1);
		chances[0] = 0;

		for (int k = 0; k < num_of_rules; k++) {
			in >> rules[k];
			in >> chance;
			chances[k + 1] = chances[k]+chance;
			
		}

		wrules.insert(std::make_pair(symbol, WRule{ rules, chances }));

	}


}

