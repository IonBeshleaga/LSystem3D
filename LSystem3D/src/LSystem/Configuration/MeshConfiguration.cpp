#include "MeshConfiguration.h"

#include <iostream>

MeshConfiguration::MeshConfiguration() {
	start_pos = glm::vec3(0, 0, 0);
	start_angle = 0;
}

MeshConfiguration::MeshConfiguration(std::string mesh_path, std::string color_config) {
	load_mesh_config(mesh_path);
	load_color_config(color_config);
}

MeshConfiguration::~MeshConfiguration() {

}

MRule MeshConfiguration::getMRule(char symbol) {
	return mrules[symbol];
}
glm::vec3 MeshConfiguration::getCRule(char symbol) {
	float r = (rand() % 10000) / 100.f;

	std::vector<float> c = crules[symbol].chances;

	int slot;
	float cur_chance = 0;
	for (slot = 0; slot < c.size(); slot++) {
		cur_chance += c[slot];
		if (r <= cur_chance) break;
	}

	return crules[symbol].color[slot];
}

void MeshConfiguration::load_mesh_config(std::string path) {
	std::ifstream in(path);

	int num_of_sym;
	char symbol, type;

	std::vector<float> v;

	in >> start_pos.x >> start_pos.y >> start_pos.z;
	in >> start_angle;
	in >> num_of_sym;


	for (int i = 0; i < num_of_sym; i++) {
		in >> symbol;
		in >> type;
		if (type == 's') {
			v.resize(1);
			in >> v[0];
			mrules.insert(std::make_pair(symbol, MRule{ stack, v }));
		}
		else if (type == 'r') {
			v.resize(6);
			in >> v[0] >> v[1] >> v[2] >> v[3] >> v[4] >> v[5];
			mrules.insert(std::make_pair(symbol, MRule{ rotate, v }));
		}
		else if (type == 'b') {
			v.resize(1);
			in >> v[0];
			mrules.insert(std::make_pair(symbol, MRule{ branch, v }));
		}
		else if (type == 'l') {
			v.resize(1);
			in >> v[0];
			mrules.insert(std::make_pair(symbol, MRule{ leaf, v }));
		}
	}

}


void MeshConfiguration::load_color_config(std::string path) {
	std::ifstream in(path);
	int num_of_sym, num_of_colors;
	float chance;
	char symbol;
	std::vector<glm::vec3> colors;
	std::vector<float> chances;
	in >> num_of_sym;



	for (int i = 0; i < num_of_sym; i++) {
		in >> symbol;
		in >> num_of_colors;
		colors.clear(); chances.clear();
		colors.resize(num_of_colors); chances.resize(num_of_colors);
		

		for (int k = 0; k < num_of_colors; k++) {
			in >> colors[k].x >> colors[k].y >> colors[k].z;
			in >> chances[k];
			colors[k] /= 255.f;
		}
		crules.insert(std::make_pair(symbol, CRule{ colors, chances }));

	}
}

