#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "../ShaderProgram/Shader.h"

class Camera {
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 Up = glm::vec3(0.f, 1.f, 0.f);
	glm::mat4 CameraMat = glm::mat4(1.f);

	bool firstClick = true;

	int width;
	int height;

	float speedNormal = 10.f;
	float speedShift = 40.f;

	float speed = speedNormal;
	float sensitivity = 10000.f;


	Camera(int width, int height, glm::vec3 position);
	void Update(float FOVdeg, float nearPlane, float farPlanel,float window_width, float window_height);
	void Matrix(Shader& shader, const char* uniform);
	void Input(GLFWwindow* window,float deltaTime);

};

#endif
