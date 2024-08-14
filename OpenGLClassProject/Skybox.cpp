#include "Skybox.h"
#include "glad.h"
#include <assert.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <vector>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include <stb_image.h>
#include <iostream>

void Skybox::IntialiseCubeMap()
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

	Vertex vertices[36];
	//Quad 1
	vertices[0].position = { -1.0f, 1.0f, -1.0f, 1 };
	vertices[1].position = { -1.0f, -1.0f, -1.0f, 1 };
	vertices[2].position = { 1.0f, -1.0f, -1.0f, 1 };

	vertices[3].position = { 1.0f, -1.0f, -1.0f, 1 };
	vertices[4].position = { 1.0f,  1.0f, -1.0f, 1 };
	vertices[5].position = { -1.0f,  1.0f, -1.0f, 1 };

	//Quad 2
	vertices[6].position = { -1.0f, -1.0f,  1.0f, 1 };
	vertices[7].position = { -1.0f, -1.0f, -1.0f, 1 };
	vertices[8].position = { -1.0f,  1.0f, -1.0f, 1 };

	vertices[9].position = { -1.0f,  1.0f, -1.0f, 1 };
	vertices[10].position = { -1.0f,  1.0f,  1.0f, 1 };
	vertices[11].position = { -1.0f, -1.0f,  1.0f, 1 };

	//Quad 3
	vertices[12].position = { 1.0f, -1.0f, -1.0f, 1 };
	vertices[13].position = { 1.0f, -1.0f,  1.0f, 1 };
	vertices[14].position = { 1.0f,  1.0f,  1.0f, 1 };

	vertices[15].position = { 1.0f,  1.0f,  1.0f, 1 };
	vertices[16].position = { 1.0f,  1.0f, -1.0f, 1 };
	vertices[17].position = { 1.0f, -1.0f, -1.0f, 1 };

	//Quad 4
	vertices[18].position = { -1.0f, -1.0f,  1.0f, 1 };
	vertices[19].position = { -1.0f,  1.0f,  1.0f, 1 };
	vertices[20].position = { 1.0f,  1.0f,  1.0f, 1 };

	vertices[21].position = { 1.0f,  1.0f,  1.0f, 1 };
	vertices[22].position = { 1.0f, -1.0f,  1.0f, 1 };
	vertices[23].position = { -1.0f, -1.0f,  1.0f, 1 };

	//Quad 5
	vertices[24].position = { -1.0f,  1.0f, -1.0f, 1 };
	vertices[25].position = { 1.0f,  1.0f, -1.0f, 1 };
	vertices[26].position = { 1.0f,  1.0f,  1.0f, 1 };

	vertices[27].position = { 1.0f,  1.0f,  1.0f, 1 };
	vertices[28].position = { -1.0f,  1.0f,  1.0f, 1 };
	vertices[29].position = { -1.0f,  1.0f, -1.0f, 1 };

	//Quad 6
	vertices[30].position = { -1.0f, -1.0f, -1.0f, 1 };
	vertices[31].position = { -1.0f, -1.0f,  1.0f, 1 };
	vertices[32].position = { 1.0f, -1.0f, -1.0f, 1 };

	vertices[33].position = { 1.0f, -1.0f, -1.0f, 1 };
	vertices[34].position = { -1.0f, -1.0f,  1.0f, 1 };
	vertices[35].position = { 1.0f, -1.0f,  1.0f, 1 };
	//std::reverse(vertices, vertices + 36);

	//fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

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

	//cube has 12 trianges
	triCount = 12;

}

unsigned int Skybox::LoadCubeMap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);


	int width, height, nrChannels;

	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void Skybox::Draw()
{
	glDepthMask(false);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);


}
