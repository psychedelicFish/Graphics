#include "Instance.h"
#include "Shader.h"
#include "Camera.h"
#include "Application.h"
#include "Mesh.h"
#include "glm/ext.hpp"
#include "Scene.h"

Instance::~Instance()
{
}

void Instance::Draw(Scene* scene)
{
    //bind shader
    shader->bind();
    //Bind our light 
    shader->bindUniform("LightDirection", scene->GetLight().direction);
    shader->bindUniform("LightColour", scene->GetLight().colour);
    shader->bindUniform("AmbientColour", scene->GetAmbientLight());

    //Bind our camera position
    shader->bindUniform("CameraPosition", scene->GetCamera()->GetPosition());

    //Bind our lights 
    int numLights = scene->GetNumberLights();
    shader->bindUniform("numLights", numLights);
    shader->bindUniform("PointLightPosition", numLights, scene->GetPointPositions());
    shader->bindUniform("PointLightColour", numLights, scene->GetPointColours());


    //Quad Stuff
    //bind transform
    auto pvm = scene->GetProjectionView() * transform;
    shader->bindUniform("ProjectionViewModel", pvm);
    shader->bindUniform("ModelMatrix", transform);

    
    glBindTexture(GL_TEXTURE_CUBE_MAP, scene->GetSkyboxTextureID());
    auto location = glGetUniformLocation(shader->getHandle(), "skyboxTex");
    glUniform1i(location, 4);
    mesh->ApplyMaterial(shader);
    //draw quad
    mesh->Draw();



}

void Instance::MakeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
{
    transform  = glm::translate(glm::mat4(1), position) * glm::rotate(glm::mat4(1), glm::radians(eulerAngles.z), glm::vec3(0, 0, 1)) *
        glm::rotate(glm::mat4(1), glm::radians(eulerAngles.y), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(eulerAngles.x), glm::vec3(1, 0, 0)) *
        glm::scale(glm::mat4(1), scale);
}
