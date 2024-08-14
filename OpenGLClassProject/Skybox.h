#pragma once
#include "Mesh.h"
class Skybox :
    public Mesh
{

public:
    void IntialiseCubeMap();
    unsigned int LoadCubeMap(std::vector<std::string> faces);

    void Draw() override;
};

