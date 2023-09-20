#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include "../VBO/VBO.h"

class VAO {
public:	
	GLuint ID;

	VAO();
	~VAO();

	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponent, GLenum type, GLboolean normalized, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();

};

#endif