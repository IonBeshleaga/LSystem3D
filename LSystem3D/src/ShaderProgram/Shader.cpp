#include "Shader.h"


std::string get_file_contest(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}throw(errno);
}

Shader::Shader(const char* vertexSourceFilename, const char* fragmentSourceFilename) {
	std::string vertexSourceStr = get_file_contest(vertexSourceFilename);
	std::string fragmentSourceStr = get_file_contest(fragmentSourceFilename);

	const char* vertexSource = vertexSourceStr.c_str();
	const char* fragmentSource = fragmentSourceStr.c_str();

	// creating Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	// creating Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	//creating shaderProgram
	ID = glCreateProgram();

	//attaching vertex and fragment shader to shaderProgram
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	//compiling
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");
	//deleting shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}


void Shader::compileErrors(GLuint shader, const char* type) {
	char InfoLog[512];
	int Status;
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &Status);
		if (Status == GL_FALSE) {
			glGetShaderInfoLog(shader, 512, NULL, InfoLog);
			std::cerr << "SHADER_COMPILATION_ERROR for " << type << "\n" << InfoLog << std::endl;
		}

	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &Status);
		if (Status == GL_FALSE) {
			glGetProgramInfoLog(shader, 512, NULL, InfoLog);
			std::cerr << "SHADER_COMPILATION_ERROR for " << type << "\n" << InfoLog << std::endl;
		}
	}

}

void Shader::setBool(const char* name, bool value) {
	glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::setFloat(const char* name, float value) {
	glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::setVec3f(const char* name, float x, float y, float z) {
	glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}

void Shader::setVec3f(const char* name, glm::vec3 vector) {
	glUniform3f(glGetUniformLocation(ID, name), vector.x, vector.y, vector.z);
}

void Shader::setMat4f(const char* name, int count, bool transpos, const GLfloat* value) {
	glUniformMatrix4fv(glGetUniformLocation(ID, name), count, transpos, value);
}