#pragma once

#include "../shader.h"
#include "texturevao.h"

class TextureShader : public Shader
{
public:
    TextureShader();

    void updateViewport(int width, int height, GLuint fbo, GLuint default_fbo);

    void updateTexture(GLuint texID);

    void createShader(int width, int height, GLuint fbo);

    void draw();

    void deleteShader();

private:
    GLuint id_texture;
    TextureVAO fullscreenQuad;
};
