#include "RulesConfiguration.h"


RulesConfiguration::RulesConfiguration(std::string lsystem_path, std::string mesh_path, std::string color_path) {
	
	load_lsystem_config(lsystem_path);
	/*load_lsystem_config(mesh_path);
	load_lsystem_config(color_path);*/
	
}

RulesConfiguration::RulesConfiguration() {
}


RulesConfiguration::~RulesConfiguration() {


}

std::string RulesConfiguration::getWRule(char symbol) {
	float r = (rand() % 10000)/100.f;

	std::cout << "Random " << r << std::endl;

	std::vector<float> c=wrules[symbol].chances;

	std::cout << "Size " << c.size() - 2 << std::endl;

	int slot;
	for (slot = 0; slot < c.size()-2; slot++) {
		if ((c[slot] <= r) && (r < c[slot + 1])) break;
	}

	std::cout << "Slot " << slot << std::endl;

	return wrules[symbol].rules[slot] ;
}
/*
mrule_data RulesConfiguration::getMRule(char symbol) {

}

glm::vec3 RulesConfiguration::getCRule(char symbol) {

}
*/
void RulesConfiguration::load_lsystem_config(std::string path) {
	int num_of_sym, num_of_rules;
	float chance;
	char symbol;
	std::vector<std::string> rules;
	std::vector<float> chances;
	std::ifstream in(path);
	
	in >> iteration;
	in >> axiom;
	in >> num_of_sym ; 
	std::cout << "num of sym " << num_of_sym << std::endl;
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
			std::cout << symbol << " chance " << k << " " << chances[k + 1];
		}

		wrules.insert(std::make_pair(symbol, WRule{ rules, chances }));

	}

	std::cout << "iterattion " << iteration << std::endl;
	std::cout << "axiom " << axiom << std::endl;
	std::cout << "Size of map " << wrules.size() << std::endl;

}

void RulesConfiguration::load_mesh_config(std::string path) {

}

void RulesConfiguration::load_color_config(std::string path) {

}

