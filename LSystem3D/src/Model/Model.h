#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include "Mesh.h"

class Model {
public:
	std::vector<Mesh> Meshes;
	Model(std::vector<Mesh>& meshes);
	~Model();

	void Draw(Shader& shader, Camera& camera);

};

#endif