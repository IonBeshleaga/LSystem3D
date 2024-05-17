#include "LSystem.h"

#include <iostream>

LSystem::LSystem() {
	
	axiom = "BBBA";
	lsystem_word = axiom;

	ls_iteration = 5;

}

LSystem::LSystem(std::string lsystem_confg_path) {
	load_lsystem_configuration(lsystem_confg_path);
}

LSystem::~LSystem() {

}

void LSystem::GenerateLSystem() {
	lsystem_word = axiom;
	for (int k = 0; k < ls_iteration; k++) {
		std::string tmp = "";
		for (int i = 0; i < lsystem_word.length(); i++) {
			tmp += rconfig.getWRule(lsystem_word[i]);
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
	lsystem_word = axiom;
}

void LSystem::setRulesConfiguration(RulesConfiguration rulesConfiguration) {
	rconfig = rulesConfiguration;
	ls_iteration = rconfig.iteration;
	axiom = rconfig.axiom;
	lsystem_word = axiom;
}

RulesConfiguration LSystem::getRulesConfiguration() {
	return rconfig;
}