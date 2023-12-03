#pragma once

#include <GL/glew.h>

class Shader
{
public:
    virtual void updateViewport(int width, int height, GLuint default_fbo) = 0;

    virtual void createShader(int width, int height) = 0;

    virtual void draw() = 0;

    virtual void deleteShader() = 0;

protected:
    GLuint defaultFBO = 2;

    Shader();
    GLuint id = 0;
    int screen_width;
    int screen_height;
};
