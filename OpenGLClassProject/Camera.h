#pragma once
#include "glad.h"
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
class Camera
{
private:

	float theta;
	float phi;
	glm::vec3 position;
	float turnSpeed;


public:
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float w, float h);

	const glm::vec3 GetPosition() const { return position; }

	void Update(float dt, GLFWwindow* window);

	Camera() : Camera(0,0 ,{-10, 3, 0}, 0.1f) {}
	Camera(float sTheta, float sPhi, glm::vec3 sPosition, float ts) : theta(sTheta), phi(sPhi), position(sPosition), turnSpeed(ts){}
};

