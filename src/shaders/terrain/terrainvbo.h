#pragma once

#include "../vbo.h"

class TerrainVBO : public VBO
{
public:
    TerrainVBO();

    const int getNumVertices() const;

    void updateVertexData(const std::vector<float>& data);

    void createVBO();

    void deleteVBO();
};
