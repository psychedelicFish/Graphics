#pragma once
#include "glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Skybox.h"


class Scene;
class Instance;
class Application
{


public:
	bool Startup();
	bool Update();
	void Draw();
	void Shutdown();

	static Application* get() { return instance; }
	glm::vec2 GetMousePosition() { return mousePosition; }
	glm::vec2 GetMouseDelta() { return mousePosition - lastMousePosition; }

	//Set up mouse input
	static void SetMousePosition(GLFWwindow* window, double x, double y);

	

protected:

	const int windowWidth = 1280;
	const int windowHeight = 720;

	static Application* instance;

	glm::mat4 view;
	glm::mat4 projection;

	GLFWwindow* window;

	aie::ShaderProgram normalPhongShader;
	aie::ShaderProgram simplePhongShader;
	aie::ShaderProgram phongNoTextureShader;
	aie::ShaderProgram skyboxShader;
	
	Mesh vertexOnlyMesh;
	Mesh indicesMesh;
	Mesh bunnyMesh;
	Mesh soulSpear;
	Skybox skybox;

	unsigned int cubemapTexture;

	aie::Texture quadTexture;
	
	
	Instance* soulSpearInstance;
	Instance* quadInstance;
	Instance* bunnyInstance;

	glm::vec2 mousePosition;
	glm::vec2 lastMousePosition;

	Camera camera;

	Scene* activeScene;

	glm::vec3 ambientLight;
};

