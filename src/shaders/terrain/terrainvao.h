#pragma once

#include "utils/terraingenerator.h"
#include "../vao.h"
#include "terrainvbo.h"

class TerrainVAO : public VAO
{
public:
    TerrainVAO();

    void createVAO();

    void draw();

    void deleteVAO();

private:
    TerrainVBO vbo;
    TerrainGenerator generator;
};

