#include "Mesh.h"

#include "glad.h"
#include <assert.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <vector>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include <iostream>

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::IntialiseQuad()
{
	//check that the mesh is not intialized already
	assert(vao == 0);

	//generate buffers 
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	//bind vertex array -> mesh wrapper
	glBindVertexArray(vao);

	//bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	Vertex vertices[6];
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };

	vertices[3].position = { -0.5f, 0, -0.5f, 1 };
	vertices[4].position = { 0.5f, 0, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0, -0.5f, 1 };

	vertices[0].normal = { 0, 1, 0, 0 };
	vertices[1].normal = { 0, 1, 0, 0 };
	vertices[2].normal = { 0, 1, 0, 0 };
	vertices[3].normal = { 0, 1, 0, 0 };
	vertices[4].normal = { 0, 1, 0, 0 };
	vertices[5].normal = { 0, 1, 0, 0 };

	vertices[0].texCoord = { 0.0f, 1.f }; 
	vertices[1].texCoord = { 1.f, 1.f };
	vertices[2].texCoord = { 0.0f, 0.0f };
	vertices[3].texCoord = { 0.0f, 0.0f };
	vertices[4].texCoord = { 1.f, 1.f };
	vertices[5].texCoord = { 1.f, 0.0f };

	//fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	
	//enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//enable the second element as normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	//enable the third element as our texture cooords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//quad has 2 trianges
	triCount = 2;

}


void Mesh::Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount, unsigned int* indices)
{
	//check that the mesh is not intialized already
	assert(vao == 0);

	//generate buffers 
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	//bind vertex array -> mesh wrapper
	glBindVertexArray(vao);

	//bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Fills our vertex buffer
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//enable the second element as normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	//enable the third element as our texture cooords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

	//enable the fourth element as our tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)40);

	//bind indices if there are any 
	if (indexCount != 0) {
		glGenBuffers(1, &ibo);

		//bind indicie buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		//fill it 
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		triCount = indexCount / 3;
	}
	else {
		triCount = vertexCount / 3;
	}


	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::InitialiseFromFile(const char* fileName)
{
	//Read the vertices from the model
	const aiScene* scene = aiImportFile(fileName, 0);
	//just use the first mesh we find
	aiMesh* mesh = scene->mMeshes[0];

	//extract the indices from that first mesh
	int numFaces = mesh->mNumFaces;
	std::vector<unsigned int> indicies;

	for (int i = 0; i < numFaces; ++i) {
		indicies.push_back(mesh->mFaces[i].mIndices[0]);
		indicies.push_back(mesh->mFaces[i].mIndices[2]);
		indicies.push_back(mesh->mFaces[i].mIndices[1]);

		//generate a second triangle for our quads
		if (mesh->mFaces[i].mNumIndices == 4) {
			indicies.push_back(mesh->mFaces[i].mIndices[0]);
			indicies.push_back(mesh->mFaces[i].mIndices[3]);
			indicies.push_back(mesh->mFaces[i].mIndices[2]);


		}

	}

	//extract vertex data
	int numV = mesh->mNumVertices;
	Vertex* vertices = new Vertex[numV];

	for (int i = 0; i < numV; i++) {
		vertices[i].position = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1);
		vertices[i].normal = glm::vec4(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 0);
		//TODO UVS
		if (mesh->mTextureCoords[0]) {
			vertices[i].texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y);
		}
		else
			vertices[i].texCoord = glm::vec2(0);

		if (mesh->HasTangentsAndBitangents()) {
			vertices[i].tangent = glm::vec4(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z, 1);
		}
	}

	if (!mesh->HasTangentsAndBitangents()) {
		CalculateTangents(vertices, numV, indicies);
	}

	Initialise(numV, vertices, indicies.size(), indicies.data());

	delete[] vertices;


}

void Mesh::Draw()
{
	glBindVertexArray(vao);

	//using indices or just vertices 

	if (ibo != 0) {
		glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
	}

}

void Mesh::ApplyMaterial(aie::ShaderProgram* shader)
{
	shader->bindUniform("Ka", Ka);
	shader->bindUniform("Kd", Kd);
	shader->bindUniform("Ks", Ks);
	shader->bindUniform("SpecularPower", SpecularPower);

	if (mapKd.getPixels() != nullptr) {
		mapKd.bind(0);
		shader->bindUniform("diffuseTex", 0);
	}
	if (mapKs.getPixels() != nullptr) {
		mapKs.bind(1);
		shader->bindUniform("specularTex", 1);
	}
	if (mapBump.getPixels() != nullptr) {
		mapBump.bind(2);
		shader->bindUniform("normalTex", 2);
	}

}

void Mesh::LoadMaterial(const char* fileName)
{
	std::fstream file(fileName, std::ios::in);
	std::string line;
	std::string header;
	char buffer[256];

	//get the path part of the file name for use 
	//with relative paths for maps later 
	std::string directory(fileName);
	int index = directory.rfind('/');
	if (index != -1) {
		directory = directory.substr(0, index + 1);
		printf(directory.c_str());
	}

	

	while (!file.eof())
	{
		file.getline(buffer, 256);
		line = buffer;
		std::stringstream ss(line, std::stringstream::in | std::stringstream::out);

		if (line.find("Ka") == 0) {
			ss >> header >> Ka.x >> Ka.y >> Ka.z;
		}
		else if (line.find("Kd") == 0) {
			ss >> header >> Kd.x >> Kd.y >> Kd.z;
		}
		else if (line.find("Ks") == 0) {
			ss >> header >> Ks.x >> Ks.y >> Ks.z;
		}
		else if (line.find("Ns") == 0) {
			ss >> header >> SpecularPower;
		}
		else if (line.find("map_Kd") == 0) {
			std::string mapFileName;
			ss >> header >> mapFileName;
			mapKd.load((directory + mapFileName).c_str());
		}
		else if (line.find("map_Ks") == 0) {
			std::string mapFileName;
			ss >> header >> mapFileName;
			mapKs.load((directory + mapFileName).c_str()); 
		}
		else if (line.find("bump") == 0) {
			std::string mapFileName;
			ss >> header >> mapFileName;
			mapBump.load((directory + mapFileName).c_str());
		}
	}

}

void Mesh::CreateMaterial(glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, const char* filePath)
{
	this->Ka = Ka;
	this->Kd = Kd;
	this->Ks = Ks; 

	mapKd.load(filePath);


}



void Mesh::CalculateTangents(Vertex* vertices, unsigned int vertexCount, const std::vector<unsigned int>& indices)
{
	glm::vec4* tan1 = new glm::vec4[vertexCount * 2];
	glm::vec4* tan2 = tan1 + vertexCount;
	memset(tan1, 0, vertexCount * sizeof(glm::vec4) * 2);
	unsigned int indexCount = (unsigned int)indices.size();
	for (unsigned int a = 0; a < indexCount; a += 3) {
		long i1 = indices[a];
		long i2 = indices[a + 1];
		long i3 = indices[a + 2];
		const glm::vec4& v1 = vertices[i1].position;
		const glm::vec4& v2 = vertices[i2].position;
		const glm::vec4& v3 = vertices[i3].position;
		const glm::vec2& w1 = vertices[i1].texCoord;
		const glm::vec2& w2 = vertices[i2].texCoord;
		const glm::vec2& w3 = vertices[i3].texCoord;
		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;
		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;
		float r = 1.0F / (s1 * t2 - s2 * t1);
		glm::vec4 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r, 0);
		glm::vec4 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r, 0);
		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;
		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}
	for (unsigned int a = 0; a < vertexCount; a++) {
		const glm::vec3& n = glm::vec3(vertices[a].normal);
		const glm::vec3& t = glm::vec3(tan1[a]);
		// Gram-Schmidt orthogonalize
		vertices[a].tangent = glm::vec4(glm::normalize(t - n * glm::dot(n,
			t)), 0);
		// Calculate handedness (direction of bitangent)
		vertices[a].tangent.w = (glm::dot(glm::cross(glm::vec3(n),
			glm::vec3(t)), glm::vec3(tan2[a])) < 0.0F) ? 1.0F : -1.0F;
	}
	delete[] tan1;
}
