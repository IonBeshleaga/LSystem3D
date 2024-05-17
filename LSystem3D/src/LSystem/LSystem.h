#ifndef LSYSTEM_CLASS_H
#define LSYSTEM_CLASS_H

#include "Configuration/RulesConfiguration.h"
#include <string>
#include <map>
#include <math.h>



class LSystem {
public:
	std::string lsystem_word;
	std::string axiom;
	int ls_iteration;

	float length;
	float angle;
	float start_angle;
	glm::vec3 start_pos;
	

	RulesConfiguration rconfig;

	LSystem();
	LSystem(std::string lsystem_confg_path);
	~LSystem();

	void load_lsystem_configuration(std::string lsystem_config_path);
	void setRulesConfiguration(RulesConfiguration rconfig);
	RulesConfiguration getRulesConfiguration();
	void GenerateLSystem();
	std::string getLSystem();

};

#endif
