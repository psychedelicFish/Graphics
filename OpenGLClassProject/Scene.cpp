#include "Scene.h"
#include "Instance.h"
#include "Camera.h"
#include "imgui_glfw3.h"

Scene::~Scene()
{
    for (auto it = instances.begin(); it != instances.end(); it++) {
        delete *it;
    }
    for (auto it = pointLights.begin(); it != pointLights.end(); it++) {
        delete* it;
    }
}

void Scene::AddInstance(Instance* instance)
{
	instances.push_back(instance);
}

void Scene::AddLight(Light* light)
{
    pointLights.push_back(light);
}

const glm::mat4 Scene::GetProjection() const
{
    return camera->GetProjectionMatrix(windowSize.x, windowSize.y); 
}

const glm::mat4 Scene::GetView() const
{
    return camera->GetViewMatrix();
}

void Scene::Update()
{
    aie::ImGui_NewFrame();

    ImGui::Begin("Light Settings");
    ImGui::DragFloat3("Sunlight Direction", &sunLight.direction[0], 0.1f, -1.0f,
        1.0f);
    ImGui::DragFloat3("Sunlight Colour", &sunLight.colour[0], 0.1f, 0.0f,
        2.0f);

    //ImGui::DragFloat3("Diffuse Colour", &bunnyMesh.Kd[0], 0.1f, 0.0f,
    //    2.0f);

    //ImGui::DragFloat("Specular", &bunnyMesh.SpecularPower, 0.1f, -10.0f,
    //    1000.0f);
    ImGui::End();
}

void Scene::Draw()
{
	pv = camera->GetProjectionMatrix(windowSize.x, windowSize.y) * camera->GetViewMatrix();
	
    for (int i = 0; i < MAX_LIGHTS && i < pointLights.size(); i++) {
        pointLightPositions[i] = pointLights[i]->direction;
        pointLightColours[i] = pointLights[i]->colour;
    }
    
    
    for (auto it = instances.begin(); it != instances.end(); it++) {
		(*it)->Draw(this);
	}
    ImGui::Render();
}
