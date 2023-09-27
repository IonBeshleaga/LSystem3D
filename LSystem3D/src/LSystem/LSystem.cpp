#include "LSystem.h"

#include <iostream>

LSystem::LSystem() {
	
	axiom = "";
	lsystem_word = axiom;

	ls_iteration = 0;

}

LSystem::~LSystem() {

}

void LSystem::GenerateLSystem() {
	for (int k = 0; k < ls_iteration; k++) {
		std::string tmp = "";
		for (int i = 0; i < lsystem_word.length(); i++) {
			tmp += rules[lsystem_word[i]];
		}
		lsystem_word = tmp;

		
	}
}

std::string LSystem::getLSystem() {
	return lsystem_word;
}

void LSystem::load_lsystem_configuration(std::string lsystem_config_path) {
	rconfig = RulesConfiguration(lsystem_config_path);
	ls_iteration = rconfig.iteration;
	axiom = rconfig.axiom;
}