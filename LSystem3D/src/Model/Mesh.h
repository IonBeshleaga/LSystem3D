#pragma once

#include "../VAO/VAO.h"
#include "../EBO/EBO.h"
#include "../ShaderProgram/Shader.h"
#include "../Camera/Camera.h"
#include "../Texture/Texture.h"



class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	VAO VAO;

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	Mesh();
	~Mesh();

	void Draw(Shader& shader, Camera& camera);

	void Draw(Shader& shader, Camera& camera, GLenum mode);
};