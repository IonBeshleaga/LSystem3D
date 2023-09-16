#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

//struct for vertx data
struct Vertex {	
	glm::vec3 position;
	glm::vec3 normals;
	glm::vec3 colors;
	glm::vec3 texUv;
};

class VBO {
public:
	GLuint ID;                    // VBO OPENGL ID
		
	VBO(std::vector<Vertex>& vertices);
	~VBO();
	
	void Bind();
	void Unbind();
	void Delete();

};

#endif