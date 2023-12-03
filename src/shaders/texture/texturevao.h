#pragma once

#include "texturevbo.h"
#include "../vao.h"

class TextureVAO : public VAO
{
public:
    TextureVAO();

    void draw(GLuint texture);

    void createVAO();

    void deleteVAO();

private:
    TextureVBO vbo;
};

