#pragma once
#include "glm/glm.hpp"
#include <vector>



struct Light {
	glm::vec3 direction;
	glm::vec3 colour;

	Light(glm::vec3 position, glm::vec3 colour, float intensity) : direction(position) {
		this->colour = colour * intensity;
	}
};

class Camera;
class Instance;
class Scene
{
protected:
	static const int MAX_LIGHTS = 4;
	
	Camera* camera;
	glm::vec2 windowSize;
	
	
	glm::vec3 ambientLight;
	Light sunLight;
	std::vector<Light*> pointLights;
	glm::vec3 pointLightPositions[MAX_LIGHTS];
	glm::vec3 pointLightColours[MAX_LIGHTS];

	std::vector<Instance*> instances;

	unsigned int skyboxTextureID;
	glm::mat4 pv;

public:
	Scene(Camera* camera, glm::vec2 windowSize, Light& light, glm::vec3 ambientLight, unsigned int skyboxTextureID) : camera{ camera }, windowSize{ windowSize }, sunLight{ light }, ambientLight{ ambientLight }, skyboxTextureID{ skyboxTextureID } {}
	~Scene();
	void AddInstance(Instance* instance);
	void AddLight(Light* light);

	Camera* GetCamera() const { return camera; }
	const glm::vec2& GetWindowSize() const { return windowSize; } 
	const glm::mat4& GetProjectionView() const { return pv; }
	const glm::mat4 GetProjection() const;
	const glm::mat4 GetView() const;
	Light& GetLight()  { return sunLight; }
	const glm::vec3& GetAmbientLight() const { return ambientLight; }

	const int GetNumberLights() { return (int)pointLights.size(); }
	const glm::vec3* GetPointPositions() { return &pointLightPositions[0]; }
	const glm::vec3* GetPointColours() { return &pointLightColours[0]; }
	unsigned int GetSkyboxTextureID() const { return skyboxTextureID; }

	void Update();
	void Draw();

};

