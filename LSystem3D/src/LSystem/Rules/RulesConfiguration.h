#ifndef RULES_CONFIG_CLASS_H
#define RULES_CONFIG_CLASS_H

#include "WRule.h"
#include "MRule.h"
#include "CRule.h"

#include <string>
#include <map>
#include <stack>

class RulesConfiguration {
public:
	std::map <char, WRule> wrules;
	std::map <char, MRule> mrules;
	std::map <char, CRule> crules;
	
	std::string getWRule(char symbol);
	mrule_data getMRule(char symbol);
	glm::vec3 getCRule(char symbol);

	void load_config();

};

#endif

