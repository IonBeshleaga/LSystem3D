#include "RulesConfiguration.h"


RulesConfiguration::RulesConfiguration(std::string path) {
	
	load_config_from_text(path);
	
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

void RulesConfiguration::load_config_from_text(std::string text) {
	std::stringstream in(text);

	int num_of_sym, num_of_rules;
	char symbol;
	std::vector<std::string> rules;
	std::vector<float> chances;


	in >> iteration;
	in >> axiom;
	in >> num_of_sym;

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

void RulesConfiguration::load_config_from_file(std::string path) {
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


std::string RulesConfiguration::getConfiguration() {
	std::string result;
	result += std::to_string(iteration) + "\n";
	result += axiom + "\n";
	result += std::to_string(wrules.size()) + "\n\n";
	for (auto it : wrules) {
		result += it.first; result += "\n";
		result += std::to_string(it.second.rules.size()) + "\n";
		for (int i = 0; i < it.second.rules.size(); i++) {
			result += it.second.rules[i] + "  "
				+ std::to_string(it.second.chances[i]) + "\n";
		}
		result += "\n";
	}

	return result;
}