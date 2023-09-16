#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <stb/stb_image.h>
#include "../ShaderProgram/Shader.h"


class Texture {
public:
	GLuint ID;
	const char* type;
	GLuint unit;
	Texture(const char* filename, const char* texType, GLuint slot, GLenum format, GLenum pixelType);
	void texUnit(Shader& shader, const char* uniform, GLuint uint);
	void Bind();
	void Unbind();
	void Delete();
};

#endif

