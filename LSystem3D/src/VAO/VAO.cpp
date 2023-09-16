#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

VAO::~VAO() {
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponent, GLenum type, GLboolean normalized, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	glVertexAttribPointer(layout, numComponent, type, normalized, stride, offset); //extract data from VBO
	glEnableVertexAttribArray(layout); // Enable this layout
	VBO.Unbind();
}

void VAO::Bind() {
	glBindVertexArray(ID);
}

void VAO::Unbind(){
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}
