#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include "../VBO/VBO.h"

class EBO {
	GLuint ID;
	EBO(std::vector<GLuint>& indices);
	~EBO();

	void Bind();
	void Unbind();
	void Delete();

};


#endif