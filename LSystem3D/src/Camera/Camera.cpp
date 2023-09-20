#include "Camera.h"


Camera::Camera(int width, int height, glm::vec3 position) {
	this->width = width;
	this->height = height;
	this->Position = position;

	lastTime = glfwGetTime();
	currentTime = 0;
	deltaTime = 0;
}

void Camera::Update(float FOVdeg, float nearPlane, float farPlane) {
	glm::mat4 view = glm::lookAt(Position, Position + Orientation, Up);
	glm::mat4 proj = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	CameraMat = proj * view;
}


void Camera::Matrix(Shader& shader, const char* uniform) {
	shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(CameraMat));

}

void Camera::Input(GLFWwindow* window) {

	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		Position += speed * Orientation * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		Position += speed * -Orientation * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		Position += speed * -glm::cross(Orientation, Up) * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		Position += speed * glm::cross(Orientation, Up) * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		Position += speed * Up * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		Position += speed * -Up * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = speedShift;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
		speed = speedNormal;
	}


	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick) {
			glfwSetCursorPos(window, width / 2, height / 2);
			firstClick = false;
		}

		double mouseY, mouseX;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = (sensitivity * (float)(mouseY - (height / 2)) / height) * deltaTime;
		float rotY = (sensitivity * (float)(mouseX - (width / 2)) / width) * deltaTime;

		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		if ((!glm::angle(newOrientation, Up) <= glm::radians(5.f)) || (glm::angle(newOrientation, -Up) <= glm::radians(5.f)))
		{
			Orientation = newOrientation;
		}

		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		glfwSetCursorPos(window, width / 2, height / 2);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}