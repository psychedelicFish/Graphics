#pragma once
#include "glm/glm.hpp";
#include "Texture.h"
#include <vector>
namespace aie { class ShaderProgram; }
class Mesh
{


public:
	struct Vertex {
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
		glm::vec4 tangent;
	};

	Mesh() : triCount(0), vao(0), vbo(0), ibo(0), Ka{ 0.25, 0.25, 0.25 }, Kd{ 0.25, 0.25, 0.25 }, Ks{ 0.25, 0.25, 0.25 }, SpecularPower(2) {}
	virtual ~Mesh();

	void IntialiseQuad();
	void Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);
	void InitialiseFromFile(const char* fileName);
	virtual void Draw();

	void ApplyMaterial(aie::ShaderProgram* shader);
	void LoadMaterial(const char* fileName);
	void CreateMaterial(glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, const char* filePath);





	float SpecularPower; //tightness of specular highlights
	glm::vec3 Kd; //diffuse colour of the surface

protected:

	unsigned int triCount;
	unsigned int vao, vbo, ibo;


	glm::vec3 Ka; //ambient colour of the surface
	glm::vec3 Ks; //specular colour of the surface

	aie::Texture mapKd; //diffuse texture map
	aie::Texture mapKs; //specular texture map
	aie::Texture mapBump; //normal texture map


protected:

	void CalculateTangents(Vertex* vertices, unsigned int vertexCount, const std::vector<unsigned int>& indices);



};

