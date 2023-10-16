#include "MeshGenerator.h"


//in gradus
float generateAnglesInRange(float a, float b) {
	if (a == b) return a;
	return a + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (b - a)));
}

void generateSection(std::vector<Vertex>& v, glm::vec3 pos, int n, float radius, float cur_angle, float cur_angle_z, glm::vec3 cur_color) {
	//std::cout << " gen_section() " << std::endl;
	float cur_in_angle = glm::radians(cur_angle);

	float rad_angle_z = glm::radians(cur_angle_z - 90);
	cur_angle_z = glm::radians(cur_angle_z);

	
	float in_angle = glm::radians(360.f / (float)n);
	glm::vec3 new_pos;
	glm::vec3 dir;
	glm::vec3 radVec;
	float shade = 0.25f; float shade_change = 0.75 / n;
	float rad_cos_z = std::cos(rad_angle_z);
	float rad_sin_z = std::sin(rad_angle_z);
	float cur_cos_z = std::cos(cur_angle_z);
	float cur_sin_z = std::sin(cur_angle_z);
	float cur_cos = std::cos(cur_in_angle);
	float cur_sin = std::sin(cur_in_angle);

	dir = glm::normalize(pos - glm::vec3(cur_cos_z*cur_cos, cur_sin_z, cur_cos_z*cur_sin));

	for (int vertices = 0; vertices < n; vertices++, cur_in_angle += in_angle,shade += shade_change) {
		
		cur_cos = std::cos(cur_in_angle);
		cur_sin = std::sin(cur_in_angle);
		
		// radVec = radVec * cos(theta) + (glm::cross(rotAxis, radVec)) * sin(theta) + rotAxis * (glm::dot(rotAxis, radVec))* (1 - cos(theta)); 
		/*
		radVec = glm::vec3(radius, 0, 0);
		radVec = radVec * cur_cos + (glm::cross(dir, radVec) * cur_sin) + dir * (glm::dot(dir, radVec) * (1 - cur_cos));
		
		new_pos = pos + radVec;*/

		new_pos = pos + glm::vec3(radius * rad_cos_z * cur_cos, radius * rad_sin_z, radius * rad_cos_z * cur_sin);
		v.push_back({ new_pos, glm::vec3(0,0,0), cur_color*shade, glm::vec2(0.0) });
	}
}
/*
void generateIndicesForSection(std::vector<GLuint>& v, int n, int start) {
	//std::cout << "Hello from ind_sect() " << start << std::endl;
	//GLuint start = v[v.size() - 1]+1;
	for (int i = 0; i < n; i++) v.push_back(start + i);
	
}
*/

void generateIndicesForBranch(std::vector<GLuint>& v, GLuint bot_ind, GLuint top_ind, int n) {
	//std::cout << "Hello from gen_ind_for_br " << bot_ind << ' ' << top_ind << std::endl;
	int i;
	for (i = 0; i < (n - 1); i++) {
		v.push_back(bot_ind + i); v.push_back(top_ind + i); v.push_back(top_ind + i + 1);
		v.push_back(bot_ind + i); v.push_back(top_ind + i +1); v.push_back(bot_ind + i + 1);
	}
	v.push_back(bot_ind+n-1); v.push_back(top_ind+n-1);	v.push_back(top_ind);
	v.push_back(bot_ind+n-1); v.push_back(top_ind);	v.push_back(bot_ind);

}

void generateIndicesForLeaf(std::vector<GLuint>& v, GLuint bot_ind, GLuint top_ind, int n) {
	int i;
	for (i = 0; i < (n - 1); i++) {
		v.push_back(bot_ind + i); v.push_back(top_ind); v.push_back(bot_ind + i + 1);
	}
	v.push_back(bot_ind + n - 1); v.push_back(top_ind);	v.push_back(bot_ind);

}

MeshGenerator::MeshGenerator() {
	start_angle = 0;
	start_pos = glm::vec3(0, 0, 0);
}

MeshGenerator::MeshGenerator(std::string mesh_config_path, std::string color_config_path) {
	load_mesh_configuration(mesh_config_path, color_config_path);
}


MeshGenerator::~MeshGenerator() {

}

void MeshGenerator::load_mesh_configuration(std::string mesh_config_path, std::string color_config_path) {
	mconfig = MeshConfiguration(mesh_config_path, color_config_path);
	start_angle = mconfig.start_angle;
	start_pos = mconfig.start_pos;
}


//MAIN GEN FUNCTION
void MeshGenerator::GenerateMesh(std::string lsystem) {
	float length = 0;
	float angle_x, angle_y, angle_z;
	glm::vec3 curColor;

	glm::vec3 CurPos = start_pos;
	glm::vec3 Dir = glm::vec3(1, 0, 0);
	glm::vec3 Translate = Dir;
	glm::vec3 RadiusVector = glm::vec3(radius, 0, 0);
	glm::quat Rotation = glm::quat(glm::vec3(0, 0, glm::radians(start_angle)));

	int last_skel_ind = 0;
	int cur_skel_ind = 0;

	Dir = glm::rotate(Rotation, Dir);
	skeleton_vertices.push_back(Vertex{ CurPos });

	for (int i = 0; i < lsystem.length(); i++) {
		MRule currentRule = mconfig.getMRule(lsystem[i]);
		switch (currentRule.type)
		{
		case stack:
			if (currentRule.data[0] == save) {
				stk.push({ CurPos, RadiusVector, Dir, last_skel_ind });
			}
			else {
				CurPos = stk.top().pos;
				RadiusVector = stk.top().radius;
				Dir= stk.top().dir;
				last_skel_ind = stk.top().indice;
				stk.pop();
			}
			break;
		case branch:
			//rule data
			length = currentRule.data[0];
			curColor = mconfig.getCRule(lsystem[i]);
			//apply data
			

			//skel vetices
			CurPos += Dir*length;
			skeleton_vertices.push_back(Vertex{ CurPos, glm::vec3(0,0,0), curColor });
			//skel indices
			skeleton_indices.push_back(last_skel_ind);
			skeleton_indices.push_back(++cur_skel_ind);
			last_skel_ind = cur_skel_ind;
			break;
		case leaf:
			//rule data
			length = currentRule.data[0];
			curColor = mconfig.getCRule(lsystem[i]);
			//apply data
			//skel vetices
			CurPos += Dir*length;
			skeleton_vertices.push_back(Vertex{ CurPos, glm::vec3(0,0,0),curColor });
			//skel indices
			skeleton_indices.push_back(last_skel_ind);
			skeleton_indices.push_back(++cur_skel_ind);
			last_skel_ind = cur_skel_ind;
			break;
		case rotate:
			angle_x = generateAnglesInRange(currentRule.data[0], currentRule.data[1]);
			angle_y = generateAnglesInRange(currentRule.data[2], currentRule.data[3]);
			angle_z = generateAnglesInRange(currentRule.data[4], currentRule.data[5]);
			Rotation = glm::quat(glm::vec3(glm::radians(angle_x), glm::radians(angle_y), glm::radians(angle_z)));
			Dir = glm::rotate(Rotation, Dir);
			std::cout << "Rotate " << Dir.x << ' ' << Dir.y << ' ' << Dir.z << std::endl;
			break;
		default:
			break;
		}
	}
}
/*void MeshGenerator::GenerateMesh(std::string lsystem) {
	//std::cout << "Starterd mesh generating" << std::endl;
	GLuint cur_skeleton_ind = 0;
	GLuint last_skeleton_ind = 0;
	GLuint cur_skin_ind = 0;
	GLuint last_skin_ind = 0;
	int tree_level = 1;
	float cur_angle = start_angle;
	float cur_angle_z = start_angle;
	glm::vec3 cur_skeleton_pos = start_pos;
	float angle_rad, angle_rad_z, add_angle;
	float cur_radius =radius; // *radius_change;
	int leaf_num = 0;

	//vertices.push_back({ cur_pos, glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec2(0,0) });

	bool first = true;
	std::cout << "LSystem size " << lsystem.length() << std::endl;
	for (int i = 0; i < lsystem.length(); i++) {
		//
		MRule currentRule = mconfig.getMRule(lsystem[i]);

		if (currentRule.type == branch) {
			angle_rad = glm::radians(cur_angle);
			angle_rad_z = glm::radians(cur_angle_z);

			float cur_cos = std::cos(angle_rad);
			float cur_sin = std::sin(angle_rad);
			float cur_cos_z = std::cos(angle_rad_z);
			float cur_sin_z = std::sin(angle_rad_z);
			
			float length = currentRule.data[0];
			glm::vec3 curColor = mconfig.getCRule(lsystem[i]);

			//std::cout << "Color " << color.x << ' ' << color.y << ' ' << color.z << std::endl;
			if (!first) {
				//skeleton mesh
				cur_skeleton_pos += glm::vec3(length * cur_cos_z*cur_cos, length * cur_sin_z, length*cur_cos_z*cur_sin);
				skeleton_vertices.push_back(Vertex{ cur_skeleton_pos, glm::vec3(0,0,0),curColor, glm::vec2(0,0) });
							
				skeleton_indices.push_back(last_skeleton_ind);
				skeleton_indices.push_back(++cur_skeleton_ind);
				last_skeleton_ind = cur_skeleton_ind;

				//skin mesh
				generateSection(skin_vertices, cur_skeleton_pos, section_size, cur_radius, cur_angle, cur_angle_z, curColor);
				
				cur_skin_ind = (section_size * cur_skeleton_ind) - (section_size - 1) * leaf_num;
				//generateIndicesForSection(skin_indices, section_size, cur_skin_ind);
				generateIndicesForBranch(skin_indices, last_skin_ind, cur_skin_ind, section_size);
				last_skin_ind = cur_skin_ind;
				
			}
			else {
				//gen first section 
				generateSection(skin_vertices, cur_skeleton_pos, section_size, cur_radius, cur_angle, cur_angle_z, curColor);
				//for (int i = 0; i < section_size; i++) skin_indices.push_back(i);

				//skel mesh
				skeleton_vertices.push_back(Vertex{ cur_skeleton_pos, glm::vec3(0,0,0),curColor, glm::vec2(0,0) });
				cur_skeleton_pos += glm::vec3(length * cur_cos_z * cur_cos, length * cur_sin_z, length * cur_cos_z * cur_sin);
				skeleton_vertices.push_back(Vertex{ cur_skeleton_pos, glm::vec3(0,0,0),curColor, glm::vec2(0,0) });
				skeleton_indices.push_back(last_skeleton_ind);
				skeleton_indices.push_back(++cur_skeleton_ind);
				last_skeleton_ind = cur_skeleton_ind;
			
				//skin mesh
				generateSection(skin_vertices, cur_skeleton_pos, section_size, cur_radius, cur_angle, cur_angle_z, curColor);
				
				cur_skin_ind = (section_size * cur_skeleton_ind) - (section_size-1)*leaf_num;
				std::cout << "cur_skin_ind " << cur_skin_ind << std::endl;
				//generateIndicesForSection(skin_indices, section_size, cur_skin_ind );
				generateIndicesForBranch(skin_indices, last_skin_ind, cur_skin_ind, section_size);
				last_skin_ind = cur_skin_ind;
				first = false;
			}

		}
		else if (currentRule.type == leaf) {
			angle_rad = glm::radians(cur_angle);
			angle_rad_z = glm::radians(cur_angle_z);

			float cur_cos = std::cos(angle_rad);
			float cur_sin = std::sin(angle_rad);
			float cur_cos_z = std::cos(angle_rad_z);
			float cur_sin_z = std::sin(angle_rad_z);

			float length = currentRule.data[0];
			glm::vec3 curColor = mconfig.getCRule(lsystem[i]);

			//std::cout << "Color " << color.x << ' ' << color.y << ' ' << color.z << std::endl;
			if (!first) {
				//skeleton mesh
				cur_skeleton_pos += glm::vec3(length * cur_cos_z * cur_cos, length * cur_sin_z, length * cur_cos_z * cur_sin);
				skeleton_vertices.push_back(Vertex{ cur_skeleton_pos, glm::vec3(0,0,0),curColor, glm::vec2(0,0) });

				skeleton_indices.push_back(last_skeleton_ind);
				skeleton_indices.push_back(++cur_skeleton_ind);
				last_skeleton_ind = cur_skeleton_ind;

				//skin mesh
				skin_vertices.push_back(Vertex{ cur_skeleton_pos, glm::vec3(0,0,0),curColor, glm::vec2(0,0) });

				cur_skin_ind = (section_size * cur_skeleton_ind) - (section_size - 1) * leaf_num;
				//generateIndicesForSection(skin_indices, section_size, cur_skin_ind);
				generateIndicesForLeaf(skin_indices, last_skin_ind, cur_skin_ind, section_size);
				last_skin_ind = cur_skin_ind;

			}
			else {
				//gen first section 
				generateSection(skin_vertices, cur_skeleton_pos, section_size, cur_radius, cur_angle, cur_angle_z, curColor);
				//for (int i = 0; i < section_size; i++) skin_indices.push_back(i);

				//skel mesh
				skeleton_vertices.push_back(Vertex{ cur_skeleton_pos, glm::vec3(0,0,0),curColor, glm::vec2(0,0) });
				cur_skeleton_pos += glm::vec3(length * cur_cos_z * cur_cos, length * cur_sin_z, length * cur_cos_z * cur_sin);
				skeleton_vertices.push_back(Vertex{ cur_skeleton_pos, glm::vec3(0,0,0),curColor, glm::vec2(0,0) });
				skeleton_indices.push_back(last_skeleton_ind);
				skeleton_indices.push_back(++cur_skeleton_ind);
				last_skeleton_ind = cur_skeleton_ind;

				//skin mesh
				skin_vertices.push_back(Vertex{ cur_skeleton_pos, glm::vec3(0,0,0),curColor, glm::vec2(0,0) });

				cur_skin_ind = (section_size * cur_skeleton_ind) - (section_size - 1) * leaf_num;
				std::cout << "cur_skin_ind " << cur_skin_ind << std::endl;
				//generateIndicesForSection(skin_indices, section_size, cur_skin_ind );
				generateIndicesForLeaf(skin_indices, last_skin_ind, cur_skin_ind, section_size);
				last_skin_ind = cur_skin_ind;
				first = false;
			}
			leaf_num++;
			
		}
		else if (currentRule.type == rotate) {
			//std::cout << " Rotation section " << std::endl;
			add_angle = generateAnglesInRange(currentRule.data[0], currentRule.data[1]);
			cur_angle += add_angle;
			add_angle = generateAnglesInRange(currentRule.data[0], currentRule.data[1]);
			cur_angle_z += add_angle;
		}
		else if (currentRule.type == stack) {
			//std::cout << " Stack section | " ;
			//std::cout << "type " << currentRule.data[0] << std::endl;
			if (currentRule.data[0] == 0) {
				
				stk.push(stack_data{ cur_skeleton_pos, cur_angle_z, last_skeleton_ind, last_skin_ind, tree_level });
				tree_level++;
				cur_radius = (radius_change/tree_level)*radius;
			}
			else {				
				cur_angle_z = stk.top().angle;
				last_skeleton_ind = stk.top().skeleton_indice;
				//last_skin_ind = (last_skeleton_ind * section_size)-(section_size-1)*leaf;
				last_skin_ind = stk.top().skin_indice;
				cur_skeleton_pos = stk.top().pos;
				tree_level = stk.top().level;
				stk.pop();
				tree_level++;
				cur_radius = (radius_change / tree_level) * radius;
			}
		}
	}
}*/

Mesh MeshGenerator::getSkeletonMesh() {
	std::vector<Texture> textures;
	return Mesh(skeleton_vertices, skeleton_indices, textures);
}

Mesh MeshGenerator::getSkinMesh() {
	std::vector<Texture> textures;
	return Mesh(skin_vertices, skin_indices, textures);
}
