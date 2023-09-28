#include "MeshGenerator.h"


float generateAnglesInRange(float a, float b) {
	if (a == b) return a;
	return a + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (b - a)));
}

MeshGenerator::MeshGenerator() {
	start_angle = 0;
	start_pos = glm::vec3(0, 0, 0);
}

MeshGenerator::~MeshGenerator() {

}

void MeshGenerator::load_mesh_configuration(std::string mesh_config_path, std::string color_config_path) {
	mconfig = MeshConfiguration(mesh_config_path, color_config_path);
	start_angle = mconfig.start_angle;
	start_pos = mconfig.start_pos;
}

void MeshGenerator::GenerateMesh(std::string lsystem) {
	GLuint cur_ind = 0;
	GLuint last_ind = 0;
	float cur_angle = start_angle;
	glm::vec3 cur_pos = start_pos;
	float angle_rad;

	//vertices.push_back({ cur_pos, glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec2(0,0) });

	bool first = true;

	for (int i = 0; i < lsystem.length(); i++) {
		//
		MRule currentRule = mconfig.getMRule(lsystem[i]);

		if (currentRule.type == branch) {
			angle_rad = glm::radians(cur_angle);

			float length = currentRule.data[0];
			glm::vec3 curColor = mconfig.getCRule(lsystem[i]);

			//std::cout << "Color " << color.x << ' ' << color.y << ' ' << color.z << std::endl;
			if (!first) {
				cur_pos += glm::vec3(length * std::cos(angle_rad), length * std::sin(angle_rad), 0);
				vertices.push_back(Vertex{ cur_pos, glm::vec3(0,0,0),curColor, glm::vec2(0,0) });
							
				indices.push_back(last_ind);
				indices.push_back(++cur_ind);
				last_ind = cur_ind;
				
			}
			else {
				vertices.push_back(Vertex{ cur_pos, glm::vec3(0,0,0),curColor, glm::vec2(0,0) });
				cur_pos += glm::vec3(length * std::cos(angle_rad), length * std::sin(angle_rad), 0);
				vertices.push_back(Vertex{ cur_pos, glm::vec3(0,0,0),curColor, glm::vec2(0,0) });
				indices.push_back(last_ind);
				indices.push_back(++cur_ind);
				last_ind = cur_ind;
				first = false;
			}

		}
		else if (currentRule.type == rotate) {
			float add_angle = generateAnglesInRange(currentRule.data[0], currentRule.data[1]);
			
			cur_angle += add_angle;
		}
		else if (currentRule.type == stack) {
			if (currentRule.data[0] == 0)
				stk.push(stack_data{ cur_pos, cur_angle, cur_ind });
			else {
				cur_angle = stk.top().angle;
				last_ind = stk.top().indices;
				cur_pos = stk.top().pos;
				stk.pop();
			}
		}
	}
}

Mesh MeshGenerator::getMesh() {
	std::vector<Texture> textures;
	return Mesh(vertices, indices, textures);
}