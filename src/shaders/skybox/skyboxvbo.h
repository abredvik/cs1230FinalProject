#pragma once

#include "../vbo.h"

class SkyboxVBO : public VBO
{
public:
    SkyboxVBO();

    const int getNumVertices() const;

    void updateVertexData(const std::vector<float>& data);

    void createVBO();

    void deleteVBO();
};

