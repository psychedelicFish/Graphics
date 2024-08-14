#include "Camera.h"
#include "glm/ext.hpp"
#include "Application.h"
glm::mat4 Camera::GetViewMatrix()
{
    float thetaR = glm::radians(theta);
    float phiR = glm::radians(phi);
    glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));



    return glm::lookAt(position, position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix(float w, float h)
{
    return glm::perspective(glm::pi<float>() * 0.25f, w/h, 0.1f, 1000.f);
}

void Camera::Update(float dt, GLFWwindow* window)
{
    float thetaR = glm::radians(theta);
    float phiR = glm::radians(phi);

    //Calculate out forward vector, our right vector and our up vector 

    glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
    glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
    glm::vec3 up(0, 1, 0);

    if (glfwGetKey(window, GLFW_KEY_W)) {
        position += forward * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
        position -= forward * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
        position -= right * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        position += right * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_Q)) {
        position += up * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_E)) {
        position -= up * dt;
    }

    glm::vec2 mouseDelta = Application::get()->GetMouseDelta();

    // if the right button of the mouse is held down allow for camera rotation
    if (glfwGetMouseButton(window, 1)) {
        theta += turnSpeed * mouseDelta.x;
        phi -= turnSpeed * mouseDelta.y;
    }

}
