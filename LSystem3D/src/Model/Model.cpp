#include "Model.h"


Model::Model(std::vector<Mesh>& meshes) {
	Model::Meshes = meshes;
}

Model::~Model() {

}

void Model::Draw(Shader& shader, Camera& camera) {
	for (int i = 0; i < Meshes.size(); i++) {
		Meshes[i].Draw(shader, camera);
	}
}