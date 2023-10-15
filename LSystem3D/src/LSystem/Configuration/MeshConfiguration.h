#ifndef MESH_CONFIG_CLASS_H
#define MESH_CONFIG_CLASS_H

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>

enum MRuleType { stack = 0, branch, leaf, rotate };
enum StackState { save = 0, load };

struct MRule {
	MRuleType type;
	std::vector<float> data;
};

struct CRule {
	std::vector<glm::vec3> color;
	std::vector<float> chances;
};

class MeshConfiguration {
public:
	std::map<char, MRule> mrules;
	std::map<char, CRule> crules;

	glm::vec3 start_pos;
	float start_angle;

	MeshConfiguration();
	MeshConfiguration(std::string mesh_path, std::string color_config);
	~MeshConfiguration();

	MRule getMRule(char symbol);
	glm::vec3 getCRule(char symbol);

private:
	void load_mesh_config(std::string path);
	void load_color_config(std::string path);
};

#endif
