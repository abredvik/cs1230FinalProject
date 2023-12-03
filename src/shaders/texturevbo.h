#pragma once

#include "vbo.h"

class TextureVBO : public VBO
{
public:
    TextureVBO();

    const int getNumVertices() const;

    void updateVertexData(const std::vector<float>& data);

    void createVBO();

    void deleteVBO();
};

