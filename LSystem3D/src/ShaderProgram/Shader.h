#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cerrno>

#include <glm/vec3.hpp>


class Shader {
public:
	GLuint ID;
	Shader(const char* vertexSourceFilename, const char* fragmentSourceFilename);

	void Activate();
	void Delete();

	void setBool(const char* name, bool value);
	void setFloat(const char* name, float value);
	void setVec3f(const char* name, float x, float y, float z);
	void setVec3f(const char* name, glm::vec3 vector);
	void setMat4f(const char* name, int count, bool transpos, const GLfloat* value);
private:
	void compileErrors(GLuint shader, const char* type);
};


std::string get_file_contest(const char* filename);

#endif