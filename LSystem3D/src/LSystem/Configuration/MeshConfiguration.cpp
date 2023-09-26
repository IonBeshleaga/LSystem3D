#include "MeshConfiguration.h"



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

}

void MeshConfiguration::load_mesh_config(std::string path) {

}


void MeshConfiguration::load_color_config(std::string path) {

}